using System;
using System.Numerics;
using System.Runtime.InteropServices;

namespace CodexEngine.Components
{
	[StructLayout(LayoutKind.Sequential)]
	public class TransformComponent
	{
		public Vector3 Position = new Vector3(0, 0, 0);
		public Vector3 Rotation = new Vector3(0, 0, 0);
		public Vector3 Scale = new Vector3(1, 1, 1);
	}

	[StructLayout(LayoutKind.Sequential)]
	public class TagComponent
	{
		[MarshalAs(UnmanagedType.LPStr)]
		public string Tag = "default tag";
	}
}
