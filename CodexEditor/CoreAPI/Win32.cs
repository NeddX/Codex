using System;
using System.Runtime.InteropServices;
using static CodexEditor.CoreAPI.Win32;

namespace CodexEditor.CoreAPI
{
	public static class Win32
	{
		public delegate IntPtr WndProc(IntPtr hWnd, uint msg, IntPtr wParam, IntPtr lParam);
		[DllImport("user32.dll", SetLastError = true)]
		public static extern IntPtr CreateWindowEx(uint dwExStyle, string lpClassName, string lpWindowName, uint dwStyle,
			uint x, uint y, uint nWidth, uint nHeight, IntPtr hWndParent, IntPtr hMenu, IntPtr hInstance, IntPtr lpParam);
		[DllImport("user32.dll", SetLastError = true)]
		[return: MarshalAs(UnmanagedType.Bool)]
		public static extern bool DestroyWindow(IntPtr hWnd);
		[DllImport("user32.dll", CharSet = CharSet.Unicode)]
		public static extern IntPtr GetDC(IntPtr hWnd);
		[DllImport("gdi32.dll")]
		public static extern int ChoosePixelFormat(IntPtr hdc, ref PIXELFORMATDESCRIPTOR pfd);
		[DllImport("gdi32.dll")]
		[return: MarshalAs(UnmanagedType.Bool)]
		public static extern bool SetPixelFormat(IntPtr hdc, int iPixelFormat, ref PIXELFORMATDESCRIPTOR pfd);
		[DllImport("gdi32.dll", CharSet = CharSet.Auto, SetLastError = true, ExactSpelling = true)]
		public static extern bool SwapBuffers(IntPtr hDC);
		[DllImport("user32.dll", SetLastError = true)]
		[return: MarshalAs(UnmanagedType.Bool)]
		public static extern bool IsWindowEnabled(IntPtr hwnd);

		public const uint WS_CHILD = 0x40000000;
		public const uint WS_VISIBLE = 0x10000000;
		public const uint WS_OVERLAPPEDWINDOW = 0x00CF0000;
		public const uint WS_CLIPSIBLINGS = 0x04000000;
		public const uint WS_EX_CONTROLPARENT = 0x00010000;
		public const uint COLOR_WINDOW = 5;
		public const uint IDC_ARROW = 32512;
		public const uint CS_HREDRAW = 2;
		public const uint CS_VREDRAW = 1;
		public const uint CS_OWNDC = 0x0020;
		public const uint CW_USEDEFAULT = unchecked((uint)-2147483648);
		public const int GWL_STYLE = -16;
		public const byte PFD_OVERLAY_PLANE = 1;
		public const byte PFD_UNDERLAY_PLANE = 255;
		public const byte PFD_TYPE_COLORINDEX = 1;
		public const byte PFD_TYPE_RGBA = 0;
		public const uint PFD_DRAW_TO_WINDOW = 0x00000004;
		public const uint PFD_SUPPORT_OPENGL = 0x00000020;
		public const uint PFD_DOUBLEBUFFER = 0x00000001;
		public const byte PFD_MAIN_PLANE = 0;
		public const uint HOST_ID = 0x00000002;
		public const int WM_DESTROY = 0x0002;
		public const int WM_NULL = 0x0000;
		public const int WM_CREATE = 0x0001;
		public const int WM_MOVE = 0x0003;
		public const int WM_SIZE = 0x0005;
		public const int WM_LBUTTONDOWN = 0x0201;
		public const int WM_LBUTTONUP = 0x0202;
		public const int WM_MOUSEMOVE = 0x0200;
		public const int WM_MOUSEWHEEL = 0x020A;
		public const int WM_CHAR = 0x0102;
		public const int WM_KEYUP = 0x0101;
		public const int WM_KEYDOWN = 0x0100;
		public const int WM_SYSKEYDOWN = 0x0104;
		public const int WM_SYSKEYUP = 0x0105;
		public const int WM_SYSCHAR = 0x0106;
		[DllImport("user32.dll")]
		public static extern IntPtr SendMessage(IntPtr hwnd, int msg, IntPtr wParam, IntPtr lParam);
		[DllImport("user32.dll")]
		public static extern IntPtr SetFocus(IntPtr hwnd);
		[DllImport("opengl32.dll")]
		public static extern IntPtr wglCreateContext(IntPtr hdc);
		[DllImport("opengl32.dll")]
		public static extern bool wglMakeCurrent(IntPtr hdc, IntPtr hglrc);
		[DllImport("opengl32.dll")]
		public static extern int wglChoosePixelFormat(IntPtr hdc, int[] piAttribIList, float[] pfAttribFList, uint nMaxFormats, int[] piFormats, out uint nNumFormats);
		[DllImport("opengl32.dll")]
		public static extern bool wglDeleteContext(IntPtr hglrc);
		[DllImport("user32.dll", SetLastError = true)]
		public static extern IntPtr LoadCursor(IntPtr hInstance, uint lpCursorName);
		[DllImport("user32.dll")]
		public static extern ushort RegisterClassEx([In] ref WNDCLASSEX lpwcx);
		[DllImport("user32.dll", SetLastError = true)]
		public static extern ushort RegisterClass([In] ref WNDCLASS lpWndClass);
		[DllImport("user32.dll", SetLastError = true)]
		public static extern IntPtr DefWindowProc(IntPtr hWnd, uint uMsg, IntPtr wParam, IntPtr lParam);
		[DllImport("user32.dll", SetLastError = true)]
		public static extern IntPtr SetWindowLong(IntPtr hWnd, int nIndex, uint dwNewLong);
		[DllImport("user32.dll", SetLastError = true)]
		public static extern bool SetWindowPos(
			IntPtr hWnd,
			IntPtr hWndInsertAfter,
			int X,
			int Y,
			int cx,
			int cy,
			uint uFlags);
		[DllImport("user32.dll", SetLastError = true)]
		public static extern void PostQuitMessage(int nExitCode);
		[DllImport("user32.dll", SetLastError = true)]
		public static extern bool UnregisterClass(
			[MarshalAs(UnmanagedType.LPStr)] string lpClassName,
			IntPtr hInstance);
		[StructLayout(LayoutKind.Sequential)]
		public struct WNDCLASS
		{
			public uint style;
			public WndProc lpfnWndProc;
			public int cbClsExtra;
			public int cbWndExtra;
			public IntPtr hInstance;
			public IntPtr hIcon;
			public IntPtr hCursor;
			public IntPtr hbrBackground;
			public string lpszMenuName;
			public string lpszClassName;
		}
		[StructLayout(LayoutKind.Sequential)]
		public struct WNDCLASSEX
		{
			public uint cbSize;
			public uint style;
			public IntPtr lpfnWndProc;
			public int cbClsExtra;
			public int cbWndExtra;
			public IntPtr hInstance;
			public IntPtr hIcon;
			public IntPtr hCursor;
			public IntPtr hbrBackground;
			public string lpszMenuName;
			public string lpszClassName;
			public IntPtr hIconSm;
		}
		[DllImport("user32.dll", SetLastError = true)]
		public static extern bool GetClientRect(IntPtr hWnd, ref RECT lpRect);
		[StructLayout(LayoutKind.Sequential)]
		public struct RECT
		{
			public int left;
			public int top;
			public int right;
			public int bottom;
		}
		public const uint SWP_NOZORDER = 0x0004;
		public const uint SWP_NOMOVE = 0x0002;
		public const uint SWP_NOACTIVATE = 0x0010;
		[StructLayout(LayoutKind.Sequential)]
		public struct PIXELFORMATDESCRIPTOR
		{
			/// <summary>
			/// 
			/// </summary>
			/// <param name="colorBits">Specifies the number of color bitplanes in each color buffer. For RGBA pixel types, it is the size of the color buffer, excluding the alpha bitplanes. For color-index pixels, it is the size of the color-index buffer.</param>
			/// <param name="depthBits">Specifies the depth of the depth (z-axis) buffer.</param>
			/// <param name="stencilBits">Specifies the depth of the stencil buffer.</param>
			public void Init(byte colorBits, byte depthBits, byte stencilBits)
			{
				nSize = (ushort)Marshal.SizeOf(typeof(PIXELFORMATDESCRIPTOR));
				nVersion = 1;
				dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |	PFD_DOUBLEBUFFER;
				iPixelType = PFD_TYPE_RGBA;
				cColorBits = colorBits;
				cRedBits = cRedShift = cGreenBits = cGreenShift = cBlueBits = cBlueShift = 0;
				cAlphaBits = cAlphaShift = 0;
				cAccumBits = cAccumRedBits = cAccumGreenBits = cAccumBlueBits = cAccumAlphaBits = 0;
				cDepthBits = depthBits;
				cStencilBits = stencilBits;
				cAuxBuffers = 0;
				iLayerType = PFD_MAIN_PLANE;
				bReserved = 0;
				dwLayerMask = dwVisibleMask = dwDamageMask = 0;
			}
			ushort nSize;
			ushort nVersion;
			uint dwFlags;
			byte iPixelType;
			byte cColorBits;
			byte cRedBits;
			byte cRedShift;
			byte cGreenBits;
			byte cGreenShift;
			byte cBlueBits;
			byte cBlueShift;
			byte cAlphaBits;
			byte cAlphaShift;
			byte cAccumBits;
			byte cAccumRedBits;
			byte cAccumGreenBits;
			byte cAccumBlueBits;
			byte cAccumAlphaBits;
			byte cDepthBits;
			byte cStencilBits;
			byte cAuxBuffers;
			byte iLayerType;
			byte bReserved;
			uint dwLayerMask;
			uint dwVisibleMask;
			uint dwDamageMask;
		}
		public static IntPtr CustomWndProc(IntPtr hWnd, uint msg, IntPtr wParam, IntPtr lParam)
		{
			return IntPtr.Zero;
			//return DefWindowProc(hWnd, msg, wParam, lParam);
		}
	}
}
