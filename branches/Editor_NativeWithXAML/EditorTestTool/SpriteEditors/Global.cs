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
        public static Dictionary<String, CloneInfor> cloneMap = new Dictionary<String, CloneInfor>();
        public static Dictionary<CloneInfor, EntityInfor> inforMap = new Dictionary<CloneInfor, EntityInfor>();

        public static void RefreshEditorGrid()
        {
            if (propertitiesGrid != null)
            {
                //propertitiesGrid.RefreshPropertyList();
                object obj = propertitiesGrid.SelectedObject;
                propertitiesGrid.SelectedObject = null;
                propertitiesGrid.SelectedObject = obj;
            }
        }

        public static void ChangePropertyObject(object nobj)
        {
            if (propertitiesGrid != null)
            {
                //propertitiesGrid.RefreshPropertyList();
                object obj = propertitiesGrid.SelectedObject;
                propertitiesGrid.SelectedObject = nobj;
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

    class CloneHandle
    {
        private static Dictionary<String, CloneInfor> cloneMap = new Dictionary<String, CloneInfor>();
        private static Dictionary<CloneInfor, EntityInfor> inforMap = new Dictionary<CloneInfor, EntityInfor>();

        protected CloneHandle()
        {

        }
        static CloneHandle instance = new CloneHandle();

        public static CloneHandle GetInstance()
        {
            return instance;
        }

        public bool AddClone(string name, CloneInfor clone)
        {
            if (cloneMap.ContainsKey(name))
                return false;
            cloneMap.Add(name, clone);
            return true;
        }

        public void RemoveClone(string name, CloneInfor clone)
        {
            if (cloneMap.ContainsKey(name))
                cloneMap.Remove(name);
        }

        public bool EditClone(string oldName, string newName)
        {
            if (oldName == null || newName == null)
                return false;
            if (cloneMap.ContainsKey(oldName) && !cloneMap.ContainsKey(newName))
            {
                CloneInfor clone = cloneMap[oldName];
                cloneMap.Remove(oldName);
                cloneMap.Add(newName, clone);
                return true;
            }
            return false;
        }

        public bool EditClone(CloneInfor infor, string newName)
        {
            if (cloneMap.ContainsValue(infor) && cloneMap.ContainsKey(infor.Name) && !cloneMap.ContainsKey(newName))
            {
                cloneMap.Remove(infor.Name);
                cloneMap.Add(newName, infor);
                return true;
            }
            return false;
        }

        public CloneInfor GetClone(string name)
        {
            return cloneMap[name];
        }
    }
}
