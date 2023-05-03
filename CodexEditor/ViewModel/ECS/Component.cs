using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;

namespace CodexEditor.ViewModel.ECS
{
	interface IMSComponent
	{

	}

	class Component : ViewModelBase
	{
		public Entity Parent { get; set; }

		public Component(Entity parent)
		{
			//Debug.Assert(parent != null);
			Parent = parent;
		}
	}

	abstract class MSComponent<T> : ViewModelBase, IMSComponent where T : Component
	{

	}
}
