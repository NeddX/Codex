using CodexEditor.CoreAPI;
using System;
using System.Threading;
using System.Runtime.InteropServices;
using System.Windows.Controls;
using System.Windows.Interop;

namespace CodexEditor.ViewModel
{
	public class EngineHostControl : HwndHost
	{
		private readonly Border _parent;
		private volatile IntPtr _hwnd;
		private Thread _renderThread;
		private volatile bool _running;
		int _renderWidth;
		int _renderHeight;
		private IntPtr _engineInst;

		public EngineHostControl(Border parent, int width, int height)
		{
			_parent = parent;
			_hwnd = IntPtr.Zero;
			_running = true;
			_renderWidth = width;
			_renderHeight = height;
			_engineInst = IntPtr.Zero;
            _renderThread = null;
		}

		private void EngineThread(object hwndParent)
		{
            _engineInst = EngineAPI.GetInstance();
            var props = new EngineAPI.WindowProperties()
            {
                Width = _renderWidth,
                Height = _renderHeight,
                FrameCap = 0,
                Vsync = true,
                PosX = 0,
                PosY = 0,
                Title = "",
                Flags = 0x00000004 | 0x00000020 | 0x00000002
            };
            EngineAPI.CreateWindow(_engineInst, ref props, _hwnd);

            while (_running)
            {
				HandleResize();
                EngineAPI.Update(_engineInst);
            }
            EngineAPI.Dispose(_engineInst);
        }

		private void HandleResize()
		{
            int newWidth    = (int)(Math.Ceiling(this.ActualWidth));
            int newHeight   = (int)(Math.Ceiling(this.ActualHeight));

            if (_renderWidth != newWidth || _renderHeight != newHeight)
            {
                _renderWidth = newWidth;
                _renderHeight = newHeight;
                EngineAPI.ResizeViewport(_engineInst, _renderWidth, _renderHeight);
                /*if (!resize(renderWidth, renderHeight))
                    throw new Exception(GetDllError());*/
            }
        }

		protected override HandleRef BuildWindowCore(HandleRef hwndParent)
		{
            _hwnd = Win32.CreateWindowEx(
                0,
                "static",
                "",
                Win32.WS_CHILD | Win32.WS_VISIBLE,
                0,
                0,
                (uint)(_renderWidth),
                (uint)(_renderHeight),
                hwndParent.Handle,
                (IntPtr)(Win32.HOST_ID),
                IntPtr.Zero,
                IntPtr.Zero
            );

            _renderThread = new Thread(new ParameterizedThreadStart(EngineThread));
			_renderThread.Start(hwndParent.Handle);

			return new HandleRef(this, _hwnd);
		}


        private int i = 0;
		protected override IntPtr WndProc(IntPtr hwnd, int msg, IntPtr wParam, IntPtr lParam, ref bool handled)
		{
            Console.Write($"\rInput count: {i++}   ");
            switch (msg)
            {
                case Win32.WM_LBUTTONDOWN:
                case Win32.WM_LBUTTONUP:
                case Win32.WM_MOUSEMOVE:
                case Win32.WM_KEYDOWN:
                case Win32.WM_KEYUP:
                case Win32.WM_CHAR:
                case Win32.WM_SYSKEYDOWN:
                case Win32.WM_SYSKEYUP:
                case Win32.WM_SYSCHAR:
                    Win32.SetFocus(this.Handle);
					handled = Win32.SendMessage(hwnd, msg, wParam, lParam) != IntPtr.Zero;
					break;
            }

			return base.WndProc(hwnd, msg, wParam, lParam, ref handled);
		}

		protected override void DestroyWindowCore(HandleRef hwnd)
		{
			_running = false;
			_renderThread.Join();
			Win32.DestroyWindow(hwnd.Handle);
		}
	}
}
