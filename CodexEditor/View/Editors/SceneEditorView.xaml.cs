using CodexEditor.CoreAPI;
using CodexEditor.Util;
using CodexEditor.ViewModel.GameProject;
using System;
using System.Collections.Specialized;
using System.Reflection;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

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

		private void UserControl_Loaded(object sender, RoutedEventArgs e)
		{
			this.Focus();
		}
	}
}