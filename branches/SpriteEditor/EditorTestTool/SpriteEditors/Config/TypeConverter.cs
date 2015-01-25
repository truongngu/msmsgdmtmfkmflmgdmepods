using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpriteEditor
{
    public class EntityInforConverter : ExpandableObjectConverter
    {
        public override object ConvertTo(ITypeDescriptorContext context, System.Globalization.CultureInfo culture, object value, Type destType)
        {
            if (destType == typeof(string) && value is EntityInfor)
            {
                // Cast the value to an Employee type
                EntityInfor emp = (EntityInfor)value;

                // Return department and department role separated by comma.
                return emp.ClassName;
            }
            return base.ConvertTo(context, culture, value, destType);
        }
    }

    // This is a special type converter which will be associated with the EmployeeCollection class.
    // It converts an EmployeeCollection object to a string representation for use in a property grid.
    public class MyCollectionConverter<T> : ExpandableObjectConverter
    {
        public override object ConvertTo(ITypeDescriptorContext context, System.Globalization.CultureInfo culture, object value, Type destType)
        {
            if (destType == typeof(string) && value is MyCollection<T>)
            {
                // Return department and department role separated by comma.
                return "Company's employee data";
            }
            return base.ConvertTo(context, culture, value, destType);
        }
    }
}
