using System;
using System.Runtime.InteropServices;
using CodexEngine.Components;
using CodexEngine.Structs;

namespace CodexEngine.Structs
{
	[StructLayout(LayoutKind.Sequential)]
	public struct WindowProperties
	{
		[MarshalAs(UnmanagedType.LPStr)]
		public string Title = "embedded window";
		public int Width = 1280;
		public int Height = 720;
		public int PosX = 30;
		public int PosY = 30;
		public int FrameCap = 0;
		public UInt32 Flags = 0;
		public bool Vsync = true;

		public WindowProperties(
			string title = "embedded window",
			int width = 1280,
			int height = 720,
			int posX = 0,
			int posY = 0,
			int frameCap = 0,
			UInt32 flags = 2,
			bool vsync = true
			)
		{
			Title = title;
			Width = width;
			Height = height;
			PosX = posX;
			PosY = posY;
			FrameCap = frameCap;
			Flags = flags;
			Vsync = vsync;
		}
	}

	[StructLayout(LayoutKind.Sequential)]
	public struct EntityDescriptor
	{
		public TransformComponent Transform = new TransformComponent();
		public TagComponent Tag = new TagComponent();

		public EntityDescriptor() { }
	}
}

namespace CodexEngine
{
	public static class CodexAPI
	{
		public const string ENGINE_DLL = @"CodexAPI.dll";

		// Engine API Imports
		[DllImport(ENGINE_DLL)]
		public static extern IntPtr Init();
		[DllImport(ENGINE_DLL)]
		public static extern void CreateWindow(ref Structs.WindowProperties props, nint nativeHandle = 0); // do not touch the last parameter
		[DllImport(ENGINE_DLL)]
		public static extern void Destroy();
		[DllImport(ENGINE_DLL)]
		public static extern void StartEngineThread();
		[DllImport(ENGINE_DLL)]
		public static extern void Update();
		[DllImport(ENGINE_DLL)]
		public static extern void ResizeViewport(int newWidth, int newHeight);
		[DllImport(ENGINE_DLL)]
		public static extern uint CreateEntity(EntityDescriptor desc);
		[DllImport(ENGINE_DLL)]
		public static extern void RemoveEntity(uint entityID);
	}
}