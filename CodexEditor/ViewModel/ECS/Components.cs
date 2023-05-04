using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;

namespace CodexEditor.ViewModel.ECS
{
	class TransformComponent : Component
	{
		private Vector3 _position;
		private Vector3 _rotation;
		private Vector3 _scale;
		public Vector3 Position
		{
			get => _position;
			set
			{
				if (_position != value)
				{
					_position = value;
					OnPropertyChanged();
				}
			}
		}
		public Vector3 Rotation
		{
			get => _rotation;
			set
			{
				if (_rotation != value)
				{
					_rotation = value;
					OnPropertyChanged();
				}
			}
		}
		public Vector3 Scale
		{
			get => _scale;
			set
			{
				if (_scale != value)
				{
					_scale = value;
					OnPropertyChanged();
				}
			}
		}

		public TransformComponent(Entity parent) : base(parent)
		{

		}
	}

	class TagComponent : Component
	{
		private string _tag;
		public string Tag
		{
			get => _tag;
			set
			{
				if (_tag != value)
				{
					_tag = value;
					OnPropertyChanged();
				}
			}
		}

		public TagComponent(Entity parent, string tag = "default tag") : base(parent)
		{
			Tag = tag;
		}
	}
}
