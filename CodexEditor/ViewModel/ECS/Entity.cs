using CodexEditor.CoreAPI;
using CodexEditor.Util;
using CodexEditor.ViewModel.GameProject;
using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Linq;
using System.Runtime.Serialization;
using CodexEngine;

namespace CodexEditor.ViewModel.ECS
{
    public class Entity : ViewModelBase
    {
        public const uint INVALID_ENTITY_ID = 0;

        public uint _entityID = INVALID_ENTITY_ID;
        public uint EntityID
        {
            get => _entityID;
            set
            {
                if (_entityID != value)
                {
                    _entityID = value;
                    OnPropertyChanged();
                }
            }
        }

        private bool _isActive;
        public bool IsActive
        {
            get => _isActive;
            set
            {
                if (_isActive != value)
                {
                    _isActive = value;
                    if (_isActive)
					{
						EntityID = EngineAPI.CreateEntity(this);
                        Logger.Log(MessageType.Info, $"Created entt id: {EntityID}");
					}
                    else
					{
						Logger.Log(MessageType.Info, $"Destroyed entt id: {EntityID}");
						EngineAPI.RemoveEntity(this);
					}
                }
            }
        }

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
        private bool _enabled;
        public bool Enabled
        {
            get => _enabled;
            set 
            {
                if (value != _enabled)
                {
                    _enabled = value;
                    OnPropertyChanged();
                }
            }
        }
        private bool _visible;
        public bool Visible
		{
			// TODO: do the undo redo and multi selection thing with Visible property as well
			// should be same as Enabled property
			get => _visible;
            set
            {
                if (value != _visible)
                {
                    _visible = value;
                    OnPropertyChanged();
                }
            }
        }

        public Scene ParentScene { get; set; }

		[JsonProperty("Components")]
        private ObservableCollection<Component> _components = new ObservableCollection<Component>();
        [JsonIgnore]
        public ReadOnlyObservableCollection<Component> Components { get; private set; }

        public Component GetComponent(Type type) => Components.FirstOrDefault(x => x.GetType() == type);
        public T GetComponent<T>() where T : Component => GetComponent(typeof(T)) as T;
        
        public Entity(Scene scene)
        {
            //Debug.Assert(scene != null);
            ParentScene = scene;
            _enabled = true;
            _components.Add(new TransformComponent(this));
            _components.Add(new TagComponent(this));
            OnDeserialized(new StreamingContext());
        }

        [OnDeserialized]
        private void OnDeserialized(StreamingContext ctx)
        {
            if (_components != null)
            {
				foreach (var component in _components)
					component.Parent = this;

				Components = new ReadOnlyObservableCollection<Component>(_components);
                OnPropertyChanged();
            }
		}
    }

    class MSEntityBase : ViewModelBase
    {
        private bool _enableUpdates = true;
        private bool? _enabled;
        public bool? Enabled
        {
            get => _enabled;
            set
            {
                if (_enabled != value)
                {
                    _enabled = value;
                    OnPropertyChanged();
                }
            }
        }

        private bool? _visible;
        public bool? Visible
        {
            get => _visible;
            set
            {
                if (_visible != value)
                {
                    _visible = value;
                    OnPropertyChanged();
                }
            }
        }

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

        private readonly ObservableCollection<IMSComponent> _components = new ObservableCollection<IMSComponent>();
        public ReadOnlyObservableCollection<IMSComponent> Componnets { get; }
        
        public List<Entity> SelectedEntities { get; }

        public MSEntityBase(List<Entity> entities)
        {
            Debug.Assert(entities != null && entities.Any());
            Componnets = new ReadOnlyObservableCollection<IMSComponent>(_components);
            SelectedEntities = entities;
            PropertyChanged += (s, e) =>
            {
                if (_enableUpdates) UpdateEntities(e.PropertyName);
            };
        }

        protected virtual bool UpdateEntities(string propertyName)
        {
			var property = typeof(Entity).GetProperty(propertyName);
			if (property != null)
			{
				var value = typeof(MSEntity).GetProperty(propertyName).GetValue(this);
				SelectedEntities.ForEach(x => property.SetValue(x, value));
				return true;
			}
			return false;
        }

        public static float? GetMixedValues(List<Entity> entities, Func<Entity, float> getProperty)
        {
            var value = getProperty(entities.First());
            
            foreach (var entity in entities.Skip(1))
                if (!value.IsTheSameAs(getProperty(entity))) return null;

            return value;
        }

        public static bool? GetMixedValues(List<Entity> entities, Func<Entity, bool> getProperty)
        {
            var value = getProperty(entities.First());

            foreach (var entity in entities.Skip(1))
                if (value != getProperty(entity)) return null; 
            
            return value;
        }

        public static string GetMixedValues(List<Entity> entities, Func<Entity, string> getProperty)
        {
            var value = getProperty(entities.First());

            foreach (var entity in entities.Skip(1))
                if (value != getProperty(entity))
                    return null;

            return value;
        }

        protected virtual bool UpdateMSEntities()
        {
            Enabled = GetMixedValues(SelectedEntities, new Func<Entity, bool>(x => x.Enabled));
            Name = GetMixedValues(SelectedEntities, new Func<Entity, string>(x => x.Name));

            return true;
        }

        public void Refresh()
        {
            _enableUpdates = false;
            UpdateMSEntities();
            _enableUpdates = true;
        }
    }

    class MSEntity : MSEntityBase
    { 
        public MSEntity(List<Entity> entities) : base(entities)
        {
            Refresh();
        }
    }
}
