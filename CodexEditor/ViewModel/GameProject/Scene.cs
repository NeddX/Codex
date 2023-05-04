using CodexEditor.Util;
using CodexEditor.ViewModel.ECS;
using Newtonsoft.Json;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Runtime.Serialization;
using System.Windows.Input;

namespace CodexEditor.ViewModel.GameProject
{
	public class Scene : ViewModelBase
	{
		private string _name;
		public string Name
		{
			get => _name;
			set
			{
				if (_name != value)
				{
					_name = value;
					OnPropertyChanged();
				}
			}
		}
		public Project Project { get; set; }

		private bool _isActive = true;
		//[JsonProperty(Order = 1)]
		public bool IsActive
		{
			get => _isActive;
			set
			{
				if (_isActive != value)
				{
					_isActive = value;
					OnPropertyChanged();
				}
			}
		}

		[JsonProperty("Entities")]
		private ObservableCollection<Entity> _entities = new ObservableCollection<Entity>();
		[JsonIgnore]
		public ReadOnlyObservableCollection<Entity> Entities { get; private set; }

		[JsonIgnore]
		public ICommand AddEntityCmd { get; private set; }
		[JsonIgnore]
		public ICommand RemoveEntityCmd { get; private set; }

		public Scene(Project project, string name)
		{
			//Debug.Assert(project != null);
			Project = project;
			Name = name;
			OnDeserialized(new StreamingContext());
		}

		private void AddEntity(Entity entity, int index = -1)
		{
			Debug.Assert(entity != null && !_entities.Contains(entity));
			entity.IsActive = IsActive;
			if (index == -1)
				_entities.Add(entity);
			else
			{
			}
		}

		private void RemoveEntity(Entity entity)
		{
			Debug.Assert(entity != null && _entities.Contains(entity));
			entity.IsActive = false;
			_entities.Remove(entity);
		}

		[OnDeserialized]
		private void OnDeserialized(StreamingContext ctx)
		{
			if (_entities != null)
			{
				foreach (var entt in _entities)
					entt.ParentScene = this;
				Entities = new ReadOnlyObservableCollection<Entity>(_entities);
				OnPropertyChanged();
			}

			foreach (var entity in _entities)
				entity.IsActive = IsActive;

			AddEntityCmd = new RelayCommand<Entity>(x =>
			{
				AddEntity(x);
				var entityIndex = _entities.Count - 1;
				Project.Unredoable.Add(new UnredoableAction(
					() => RemoveEntity(x),
					() => AddEntity(x, entityIndex),
					$"Add {x.Name} in {Name}"
					));
			});

			RemoveEntityCmd = new RelayCommand<Entity>(x =>
			{
				RemoveEntity(x);
				var entityIndex = _entities.Count - 1; // handle -1
				if (entityIndex < 0) return;
				Project.Unredoable.Add(new UnredoableAction(
					() => AddEntity(x, entityIndex),
					() => RemoveEntity(x),
					$"Removed {x.Name} in {Name}"
					));

			});
		}
	}
}
