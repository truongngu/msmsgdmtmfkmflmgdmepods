using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;

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
    }
}
