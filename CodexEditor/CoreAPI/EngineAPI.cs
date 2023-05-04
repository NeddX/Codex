using System;
using CodexEditor.ViewModel.ECS;
using CodexEngine;
using CodexEngine.Structs;

namespace CodexEditor.CoreAPI
{
	public static class EngineAPI
	{
		public static uint CreateEntity(Entity entity)
		{
			EntityDescriptor desc = new EntityDescriptor();

			{
				var transformComponent	= entity.GetComponent<TransformComponent>();
				desc.Transform.Position = transformComponent.Position;
				desc.Transform.Rotation = transformComponent.Rotation;
				desc.Transform.Scale	= transformComponent.Scale;

				var tagComponent		= entity.GetComponent<TagComponent>();
				desc.Tag.Tag = tagComponent.Tag;
			}

			return CodexAPI.CreateEntity(desc);
		}

		public static void RemoveEntity(Entity entity)
		{
			CodexAPI.RemoveEntity(entity.EntityID);
		}
	}
}
