using CodexEditor.Util;
using CodexEditor.View.Shared;
using CodexEditor.ViewModel;
using CodexEditor.ViewModel.ECS;
using CodexEditor.ViewModel.Editors;
using CodexEditor.ViewModel.GameProject;
using CodexEditor.ViewModel.Shared;
using CodexEngine;
using CodexEngine.Components;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Numerics;
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
using TileInfo = CodexEditor.ViewModel.Editors.TileInfo;

namespace CodexEditor.View.Editors
{
	/// <summary>
	/// Interaction logic for TileEditorView.xaml
	/// </summary>
	/// 

	sealed internal class MouseInfo
	{
		public bool IsMouseDown = false;
		public MouseButton ChangedButton;
	}

	public partial class TileEditorView : UserControl
	{
		private MouseInfo _mouseInfo = new MouseInfo();

		public TileEditorView()
		{
			InitializeComponent();

			DataContextChanged += (s, e) =>
			{
				if (DataContext is Project proj)
				{
					DataContext = new TileEditor(proj);
					EngineHostView.EngineInstance.MouseDown += EngineInstance_MouseDown;
					EngineHostView.EngineInstance.MouseUp += EngineInstance_MouseUp;
					EngineHostView.EngineInstance.MouseMove += EngineInstance_MouseMove;
				}
			};
		}

		private void EngineInstance_MouseUp(object? sender, MouseButtonEventArgs e)
		{
			_mouseInfo.IsMouseDown = false;
			_mouseInfo.ChangedButton = e.ChangedButton;
		}

		private void EngineInstance_MouseMove(object? sender, MouseEventArgs e)
		{
			if (!_mouseInfo.IsMouseDown) return;

			var da = DataContext as TileEditor;
			if (da.ActiveEntity != null)
			{
				var mousePosInWorld = new Vector2();
				CodexAPI.GetMousePositionInWorld(ref mousePosInWorld);
				mousePosInWorld = da.GetCoordSnappedToGrid(mousePosInWorld);

				switch (_mouseInfo.ChangedButton)
				{
					case MouseButton.Left:
						{
							da.Component.AddTile(mousePosInWorld, da.SelectedTileCoord);
							break;
						}
					case MouseButton.Right:
						{
							da.Component.RemoveTile(mousePosInWorld);
							break;
						}
				}
			}
		}

		private void EngineInstance_MouseDown(object? sender, MouseButtonEventArgs e)
		{
			_mouseInfo.IsMouseDown = true;
			_mouseInfo.ChangedButton = e.ChangedButton;
		}

		private void ListBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
		{
			var da = DataContext as TileEditor;
			var listBox = sender as ListBox;
			var entity = listBox.SelectedItem as Entity;
			dropMenu.Text = entity.Name;
			da.ActiveEntity = entity;
		}

		private void DropDownMenu_Click(object sender, RoutedEventArgs e)
		{
			var da = DataContext as TileEditor;
			da.RefreshEntities();
		}

		private void SpriteButton_Click(object sender, RoutedEventArgs e)
		{
			var da = DataContext as TileEditor;
			var button = sender as Button;
			var tileinfo = button.DataContext as TileInfo;
			da.NotifyTilePick(tileinfo);
		}
	}
}
