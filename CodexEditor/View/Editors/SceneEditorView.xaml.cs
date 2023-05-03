using CodexEditor.ViewModel.GameProject;
using System.Collections.Specialized;
using System.Windows.Controls;

namespace CodexEditor.View.Editors
{
	/// <summary>
	/// Interaction logic for SceneEditor.xaml
	/// </summary>
	public partial class SceneEditor : UserControl
	{
		public SceneEditor()
		{
			InitializeComponent();
		}

		private void UserControl_Loaded(object sender, System.Windows.RoutedEventArgs e)
		{
			this.Focus();
			((INotifyCollectionChanged)Project.Unredoable.UndoList).CollectionChanged += (s, e) => Focus();
		}
	}
}