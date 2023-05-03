using CodexEditor.Util;
using CodexEditor.ViewModel.ECS;
using CodexEditor.ViewModel.GameProject;
using System.Linq;
using System.Windows;
using System.Windows.Controls;

namespace CodexEditor.View.Editors
{
    /// <summary>
    /// Interaction logic for SceneHierarchy.xaml
    /// </summary>
    public partial class SceneHierarchy : UserControl
    {
        public SceneHierarchy()
        {
            InitializeComponent();
        }

		private void createEntityBtn_Click(object sender, RoutedEventArgs e)
		{
            var btn = sender as Button;
            var da = btn.DataContext as Scene;
            da.AddEntityCmd.Execute(new Entity(da) { Name = $"Empty entity {da.Entities.Count}" });
        }

		private void ListBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
		{
            var listbox = sender as ListBox;
            if (listbox.SelectedItems.Count > 0)
			{
				var selectedEntity = listbox.SelectedItems[0];
				EntityView.Instance.DataContext = selectedEntity;

				var newSelection = listbox.SelectedItems.Cast<Entity>().ToList();
                var previousSelection = newSelection.Except(
                    e.AddedItems.Cast<Entity>()).Concat(e.RemovedItems.Cast<Entity>()).ToList();

                Project.Unredoable.Add(new UnredoableAction(
                    () => 
                    {
                        listbox.UnselectAll();
                        previousSelection.ForEach(x => 
                            (listbox.ItemContainerGenerator.ContainerFromItem(x) as ListBoxItem).IsSelected = true);
                    },
                    () => 
                    {
						listbox.UnselectAll();
						newSelection.ForEach(x =>
                        {
                            var item = listbox.ItemContainerGenerator.ContainerFromItem(x) as ListBoxItem;
                            if (item != null) item.IsSelected = true;
                        });
					},
                    "Selection changed"));

                MSEntity msEntity = null;
                if (newSelection.Any())
                    msEntity = new MSEntity(newSelection);
                EntityView.Instance.DataContext = msEntity;
			}
        }

		private void UserControl_Loaded(object sender, RoutedEventArgs e)
		{

		}
	}
}
