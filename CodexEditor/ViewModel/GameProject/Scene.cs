using CodexEditor.Util;
using CodexEditor.ViewModel;
using CodexEditor.ViewModel.ECS;
using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace CodexEditor.ViewModel.GameProject
{
	class Scene : ViewModelBase
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

		private void AddEntity(Entity entity)
		{
			Debug.Assert(entity != null && !_entities.Contains(entity));
			_entities.Add(entity);
		}

		private void RemoveEntity(Entity entity)
		{
			Debug.Assert(entity != null && _entities.Contains(entity));
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

			AddEntityCmd = new RelayCommand<Entity>(x =>
			{
				AddEntity(x);
				var entityIndex = _entities.Count - 1;
				Project.Unredoable.Add(new UnredoableAction(
					() => RemoveEntity(x),
					() => _entities.Insert(entityIndex, x),
					$"Add {x.Name} in {Name}"
					));
			});

			RemoveEntityCmd = new RelayCommand<Entity>(x =>
			{
				RemoveEntity(x);
				var entityIndex = _entities.Count - 1; // handle -1
				if (entityIndex < 0) return;
				Project.Unredoable.Add(new UnredoableAction(
					() => _entities.Insert(entityIndex, x),
					() => RemoveEntity(x),
					$"Removed {x.Name} in {Name}"
					));

			});
		}
	}
}
