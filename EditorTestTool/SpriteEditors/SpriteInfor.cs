using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpriteEditor
{
    public class Vector2
    {
        float _x, _y;

        public float Y
        {
            get { return _y; }
            set { _y = value; }
        }

        public float X
        {
            get { return _x; }
            set { _x = value; }
        }
        public Vector2(float x, float y)
        {
            _x = x;
            _y = y;
        }

        public Vector2(float x)
        {
            _x = x;
            _y = x;
        }

        public Vector2()
        {
            _x = 0;
            _y = 0;
        }

        public override string ToString()
        {
            return _x + " " + _y;
        }
    }

    public class Vector3
    {
        float _x, _y, _z;

        public float Z
        {
            get { return _z; }
            set { _z = value; }
        }

        public float Y
        {
            get { return _y; }
            set { _y = value; }
        }

        public float X
        {
            get { return _x; }
            set { _x = value; }
        }

        public Vector3(float x, float y, float z)
        {
            _x = x;
            _y = y;
            _z = z;
        }

        public Vector3(float x, float y)
        {
            _x = x;
            _y = y;
            _z = 0;
        }

        public Vector3(float x)
        {
            _x = x;
            _y = x;
            _z = x;
        }

        public Vector3()
        {
            _x = 0;
            _y = 0;
            _z = 0;
        }

        public override string ToString()
        {
            return _x + " " + _y + " " + _z;
        }
    }

    public class SpriteInfor
    {
    }
}
