using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpriteEditor
{
    [TypeConverter(typeof(ExpandableObjectConverter))]
    public class MyCollection<T> : List<T>, ICustomTypeDescriptor
    {
        #region CollectionBase
        public void Add(T emp)
        {
            base.Add(emp);
        }
        public void Remove(T emp)
        {
            base.Remove(emp);
        }
        public T this[int index]
        {
            get
            {
                return (T)base[index];
            }
        }
        #endregion

        #region ICustomTypeDescriptor
        public AttributeCollection GetAttributes()
        {
            return TypeDescriptor.GetAttributes(this, true);
        }

        public string GetClassName()
        {
            return TypeDescriptor.GetClassName(this, true);
        }

        public string GetComponentName()
        {
            return TypeDescriptor.GetComponentName(this, true);
        }

        public TypeConverter GetConverter()
        {
            return TypeDescriptor.GetConverter(this, true);
        }

        public EventDescriptor GetDefaultEvent()
        {
            return TypeDescriptor.GetDefaultEvent(this, true);
        }

        public PropertyDescriptor GetDefaultProperty()
        {
            return TypeDescriptor.GetDefaultProperty(this, true);
        }

        public object GetEditor(Type editorBaseType)
        {
            return TypeDescriptor.GetEditor(this, editorBaseType, true);
        }

        public EventDescriptorCollection GetEvents(Attribute[] attributes)
        {
            return TypeDescriptor.GetEvents(this, attributes, true);
        }

        public EventDescriptorCollection GetEvents()
        {
            return TypeDescriptor.GetEvents(this, true);
        }

        public PropertyDescriptorCollection GetProperties(Attribute[] attributes)
        {
            return GetProperties();
        }

        public PropertyDescriptorCollection GetProperties()
        {
            // Create a collection object to hold property descriptors
            PropertyDescriptorCollection pds = new PropertyDescriptorCollection(null);

            // Iterate the list of employees
            for (int i = 0; i < this.Count; i++)
            {
                // Create a property descriptor for the employee item and add to the property descriptor collection
               MyCollectionPropertyDescriptor<T> pd = new MyCollectionPropertyDescriptor<T>(this, i);
                pds.Add(pd);
            }
            // return the property descriptor collection
            return pds;
        }

        public object GetPropertyOwner(PropertyDescriptor pd)
        {
            return this;
        }
        #endregion
    }
}
