using CodexEditor.Util;
using Newtonsoft.Json;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.Serialization;
using System.Windows;
using System.Windows.Input;

namespace CodexEditor.ViewModel.GameProject
{
    [JsonObject("Game")]
    public class Project : ViewModelBase
    {
        [JsonIgnore]
        public static string Extension = ".cfxproj";
        public string ProjectName { get; set; }
        public string ProjectPath { get; set; }
        [JsonIgnore]
        public string FullPath => $"{ProjectPath}{ProjectName}{Extension}";
        [JsonProperty("Scenes")]
        private ObservableCollection<Scene> _scenes = new ObservableCollection<Scene>();
        [JsonIgnore]
        public ReadOnlyObservableCollection<Scene> Scenes { get; private set; }
        public static Project Current => Application.Current.MainWindow.DataContext as Project;
        private Scene _activeScene;
        [JsonIgnore]
        public Scene ActiveScene
        {
            get => _activeScene;
            set
            {
                if (_activeScene != value)
                {
                    _activeScene = value;
                    OnPropertyChanged();
                }
            }
        }
        [JsonIgnore]
        public ICommand SaveCmd { get; private set; }
		[JsonIgnore]
		public ICommand UndoCmd { get; private set; }
		[JsonIgnore]
		public ICommand RedoCmd { get; private set; }
		[JsonIgnore]
		public ICommand AddSceneCmd { get; private set; }
		[JsonIgnore]
		public ICommand RemoveSceneCmd { get; private set; }
		[JsonIgnore]
		public static Unredoable Unredoable { get; } = new Unredoable();

		public Project(string name, string path) 
        {
            ProjectName = name;
            ProjectPath = path;

			var scene = new Scene(this, "Default Scene");
            _scenes.Add(scene);
            ActiveScene = scene;
            OnDeserialized(new StreamingContext());
        }

        public static Project LoadProject(string path)
        {
            Debug.Assert(File.Exists(path));
            return Serializer.FromFile<Project>(path);
        }

        public static void SaveProject(Project project)
        {
            var settings = new JsonSerializerSettings
            {
                PreserveReferencesHandling = PreserveReferencesHandling.Objects,
            };
            Logger.Log(MessageType.Info, "Saving project...");
            Serializer.ToFile(project.FullPath, project, Formatting.Indented, settings);
        }

        public void Unload()
        {
            Unredoable.Reset();
        }

        private void AddScene(string name)
        {
            Debug.Assert(!string.IsNullOrEmpty(name.Trim()));
            _scenes.Add(new Scene(this, name));
        }

        private void RemoveScene(Scene scene)
        {
            Debug.Assert(scene != null && _scenes.Contains(scene));
            _scenes.Remove(scene);
        }

        [OnDeserialized]
        private void OnDeserialized(StreamingContext ctx)
        {
            if (_scenes != null)
            {
                foreach (var scene in _scenes)
                    scene.Project = this;
                Scenes = new ReadOnlyObservableCollection<Scene>(_scenes);
                OnPropertyChanged();
            }
            ActiveScene = Scenes.FirstOrDefault(x => x.IsActive);

            AddSceneCmd = new RelayCommand<object>(x => 
            {
                AddScene($"New Scene {_scenes.Count}");
                var scene = _scenes.Last();
                var sceneIndex = _scenes.Count - 1;
                Unredoable.Add(new UnredoableAction(
                    () => RemoveScene(scene),
                    () => _scenes.Insert(sceneIndex, scene),
                    $"Add {scene.Name}"
                    ));
            });

            RemoveSceneCmd = new RelayCommand<Scene>(x =>
            {
                var sceneIndex = _scenes.IndexOf(x);
                RemoveScene(x);
				Unredoable.Add(new UnredoableAction(
					() => _scenes.Insert(sceneIndex, x),
					() => RemoveScene(x),
					$"Removed {x.Name}"
					));

			}, x => x.IsActive);

            UndoCmd = new RelayCommand<object>(x => Unredoable.Undo());
            RedoCmd = new RelayCommand<object>(x => Unredoable.Redo());

            SaveCmd = new RelayCommand<object>(x => SaveProject(this));
        }
    }
}
