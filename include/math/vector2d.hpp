
#ifndef ____vector2d__
#define ____vector2d__


#include <cmath>


namespace math
{
    template<class T>
    class Vector2d
    {
        static_assert(std::is_floating_point<T>::value,
                      "Vector2d can be of floating point data types only");

        T _x;
        T _y;
    public:
        const T& x;
        const T& y;
        
        Vector2d() : x(_x), y(_y)
        {
            _x = 0;
            _y = 0;
        }

        Vector2d(T x, T y) : x(_x), y(_y)
        {
            _x = x;
            _y = y;
        }

        Vector2d(const Vector2d<T> &v) : x(_x), y(_y)
        {
            _x = v.x;
            _y = v.y;
        }

        void operator=(const Vector2d<T> &v)
        {
            _x = v.x;
            _y = v.y;
        }
        
        Vector2d<T>& Set(T x, T y)
        {
            _x = x;
            _y = y;
            return *this;
        }
        
        Vector2d<T>& operator-=(const Vector2d<T>& v)
        {
            return Set(_x-v.x, _y-v.y);
        }
        
        Vector2d<T>& operator+=(const Vector2d<T>& v)
        {
            return Set(_x+v.x, _y+v.y);
        }
        
        Vector2d<T>& operator*=(const T& s)
        {
            return Set(_x*s, _y*s);
        }
        
        Vector2d<T>& operator/=(const T& s)
        {
            return Set(_x/s, _y/s);
        }
    };
    

    template<class T> Vector2d<T> operator-(const Vector2d<T>& v)
    {
        return Vector2d<T>(-v.x, -v.y);
    }

    template<class T> Vector2d<T> operator-(const Vector2d<T>& v1, const Vector2d<T>& v2)
    {
        return Vector2d<T>(v1.x-v2.x, v1.y-v2.y);
    }

    template<class T> Vector2d<T> operator+(const Vector2d<T>& v1, const Vector2d<T>& v2)
    {
        return Vector2d<T>(v1.x+v2.x, v1.y+v2.y);
    }

    template<class T> Vector2d<T> operator*(const Vector2d<T>& v, const T& s)
    {
        return Vector2d<T>(v.x*s, v.y*s);
    }

    template<class T> Vector2d<T> operator*(const T& s, const Vector2d<T>& v)
    {
        return Vector2d<T>(v.x*s, v.y*s);
    }

    template<class T> Vector2d<T> operator/(const Vector2d<T>& v, const T& s)
    {
        return Vector2d<T>(v.x/s, v.y/s);
    }

    template<class T> Vector2d<T> operator/(const T& s, const Vector2d<T>& v)
    {
        return Vector2d<T>(v.x/s, v.y/s);
    }

    template<class T> bool operator==(const Vector2d<T>& v1, const Vector2d<T>& v2)
    {
        return v1.x == v2.x && v1.y == v2.y;
    }

    template<class T> bool operator!=(const Vector2d<T>& v1, const Vector2d<T>& v2)
    {
        return !(v1 == v2);
    }

    template<class T> T DotProduct(const Vector2d<T>& v1, const Vector2d<T>& v2)
    {
        return ((v1.x * v2.x) + (v1.y * v2.y));
    }

    template<class T> T CrossProduct(const Vector2d<T>& v1, const Vector2d<T>& v2)
    {
        return ((v1.x * v2.y) - (v1.y * v2.x));
    }
    
    template<class T> T EuclideanLengthSquare(const Vector2d<T>& v)
    {
        return (v.x * v.x) + (v.y * v.y);
    }

    template<class T> T EuclideanLength(const Vector2d<T>& v)
    {
        return sqrt(EuclideanLengthSquare(v));
    }

    template<class T> Vector2d<T> Normal(const Vector2d<T>& v)
    {
        T magnitude = EuclideanLength(v);
        return Vector2d<T>(v.x / magnitude, v.y / magnitude);
    }

    template<class T> Vector2d<T> Perpendicular(const Vector2d<T>& v)
    {
        return Vector2d<T>(v.y, -v.x);
    }
    
    template<class T> T Angle(Vector2d<T>& v)
    {
        return atan2(v.y, v.x);
    }
    
    template<class T> T Angle(Vector2d<T>& start, Vector2d<T>& end)
    {
        return atan2((start.x * end.y) - (start.y * end.x), (start.x * end.x) + (start.y * end.y));
    }
    
    template<class T> T Angle(Vector2d<T>& left, Vector2d<T>& center, Vector2d<T>& right)
    {
        Vector2d<T> start = left - center;
        Vector2d<T> end = right - center;
        return Angle(start, end);
    }
    
    template<class T> bool EpsilonEquals(Vector2d<T>& v1, Vector2d<T>& v2, T epsilon)
    {
        return abs(v1.x - v2.x) <= epsilon && abs(v1.y - v2.y) <= epsilon;
    }
    
    template<class T> Vector2d<T> Rotate(Vector2d<T>& v, T radians)
    {
        T cosine = cos(radians);
        T sine = sin(radians);
        return Vector2d<T>(v.x * cosine - v.y * sine, v.x * sine + v.y * cosine);
    }
    
    template<class T> Vector2d<T> Rotate(Vector2d<T>& v, Vector2d<T>& origin, T radians)
    {
        T x = v.x - origin.x;
        T y = v.y - origin.y;
        return Vector2d<T>((x*cos(radians) - y*sin(radians) + origin.x), (x*sin(radians) + y*cos(radians) + origin.y));
    }
}

#endif /* defined(____vector2d__) */
