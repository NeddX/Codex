using CodexEditor.Util;
using CodexEditor.ViewModel.ECS;
using CodexEditor.ViewModel.GameProject;
using Newtonsoft.Json.Converters;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace CodexEditor.View.Editors
{
	/// <summary>
	/// Interaction logic for EntityView.xaml
	/// </summary>
	public partial class EntityView : UserControl
	{

		public static EntityView Instance { get; private set; }

		private Action _undoAction;
		private string _propertyName;

		public EntityView()
		{
			InitializeComponent();
			DataContext = null;
			Instance = this;
			DataContextChanged += (_, __) =>
			{
				var da = DataContext as MSEntity;
				if (da != null)
				{
					da.PropertyChanged += (s, e) =>
					{
						_propertyName = e.PropertyName;
					}; 
				}
			};
		}

		private void TextBox_GotKeyboardFocus(object sender, KeyboardFocusChangedEventArgs e)
		{
			var da = DataContext as MSEntity;
			var selection = da.SelectedEntities.Select(entity => (entity, entity.Name)).ToList();
			_undoAction = new Action(() =>
			{
				selection.ForEach(x => x.entity.Name = x.Name);
				da.Refresh();
			});
		}

		private void TextBox_LostKeyboardFocus(object sender, KeyboardFocusChangedEventArgs e)
		{
			if (_propertyName == nameof(MSEntity.Name) && _undoAction != null)
			{
				var da = DataContext as MSEntity;
				var selection = da.SelectedEntities.Select(entity => (entity, entity.Name)).ToList();
				var redoAction = new Action(() =>
				{
					selection.ForEach(item => item.entity.Name = item.Name);
					da.Refresh();
				});
				Project.Unredoable.Add(new UnredoableAction(_undoAction, redoAction, "Rename entity"));
				_propertyName = null;
			}
			_undoAction = null;
		}

		private void CheckBox_Click(object sender, RoutedEventArgs e)
		{
			var da = DataContext as MSEntity;
			var selection = da.SelectedEntities.Select(entity => (entity, entity.Enabled)).ToList();
			var undoAction = new Action(() =>
			{
				selection.ForEach(item => item.entity.Enabled = item.Enabled);
				da.Refresh();
			});
			da.Enabled = (sender as CheckBox).IsChecked == true;
			var redoAction = new Action(() => 
			{
				selection.ForEach(item => item.entity.Enabled = item.Enabled);
				da.Refresh();
			}); 

			Project.Unredoable.Add(new UnredoableAction(undoAction, redoAction, 
				(da.Enabled == true) ? "Enabled entity" : "Disabled entity"));
		}
	}
}
