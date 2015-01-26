using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

namespace SpriteEditor
{
    class Global
    {
        public static WpfPropertyGrid propertitiesGrid = null;
        public static void RefreshEditorGrid()
        {
            if (propertitiesGrid != null)
            {
                propertitiesGrid.RefreshPropertyList();
            }
        }

        public static void ExpandCloneInfor(String cloneName)
        {
            DependencyObject root=propertitiesGrid.Parent;

            int childrenCount = VisualTreeHelper.GetChildrenCount(root);
            for (int i = 0; i < childrenCount; i++)
            {
                var child = VisualTreeHelper.GetChild(root, i);
                if (child is TreeViewItem)
                {
                    var tree = (TreeViewItem)child;
                    tree.IsExpanded = true;
                }
            }
        }
    }
}
