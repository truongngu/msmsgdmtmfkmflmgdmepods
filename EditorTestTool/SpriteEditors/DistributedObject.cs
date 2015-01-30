using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpriteEditor
{
    public class MyAttribute
    {
        string _name;
        object _value;
        string _type;

        public MyAttribute(string name, object value, string type = "object")
        {
            // TODO: Complete member initialization
            this.Name = name;
            this.Value = value;
            this.Type = type;
        }

        public string Name
        {
            get { return _name; }
            set { _name = value; }
        }

        public object Value
        {
            get { return _value; }
            set { _value = value; }
        }

        public string Type
        {
            get { return _type; }
            set { _type = value; }
        }
    }
    public class DistributeObject
    {
        public DistributeObject()
        {
            AddAttribute("Value", new MyAttribute("Value", "1"), true);
        }

        Dictionary<String, MyAttribute> _attributes = new Dictionary<string, MyAttribute>();

        public Dictionary<String, MyAttribute> Attributes
        {
            get { return _attributes; }
            set { _attributes = value; }
        }

        public void AddAttribute(string name, MyAttribute value, bool replace = false)
        {
            if (_attributes.ContainsKey(name))
            {
                if (replace)
                {
                    _attributes.Remove(name);
                }
                else
                    return;
            }
            _attributes.Add(name, value);
        }

        public bool SetAttribue(string name, MyAttribute value)
        {
            if (_attributes.ContainsKey(name))
            {
                _attributes.Remove(name);
                _attributes.Add(name, value);
                return true;
            }
            else
            {
                return false;
            }
        }

        public MyAttribute GetAttribute(string name)
        {
            if (_attributes.ContainsKey(name))
            {
                return _attributes[name];
            }
            else
            {
                return null;
            }
        }
    }
}
