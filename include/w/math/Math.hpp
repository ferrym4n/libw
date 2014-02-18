/**
 * libw
 *
 * Copyright (C) 2013 Woima Solutions
 *
 * This software is provided 'as-is', without any express or implied warranty. In
 * no event will the authors be held liable for any damages arising from the use
 * of this software.
 *
 * Permission is granted to anyone to use this software for any purpose, including
 * commercial applications, and to alter it and redistribute it freely, subject to
 * the following restrictions:
 *
 * 1) The origin of this software must not be misrepresented; you must not claim
 *    that you wrote the original software. If you use this software in a product,
 *    an acknowledgment in the product documentation is appreciated.
 *
 * 2) Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 *
 * 3) This notice may not be removed or altered from any source distribution.
 *
 * @author antti.peuhkurinen@woimasolutions.com
 */

#ifndef LIBW_MATH_MATH
#define LIBW_MATH_MATH

#include <w/math/Eigen.hpp>

namespace w
{
    const float Pi = 3.1415;

    namespace math
    {
        static Eigen::Matrix4f lookAt(const Eigen::Vector3f& position, const Eigen::Vector3f& target, const Eigen::Vector3f& up)
        {
            Eigen::Matrix3f tmp;
            tmp.col(2) = (position-target).normalized();
            tmp.col(0) = up.cross(tmp.col(2)).normalized();
            tmp.col(1) = tmp.col(2).cross(tmp.col(0));

            Eigen::Matrix4f r;
            r.topLeftCorner<3, 3>() = tmp.transpose();
            r.topRightCorner<3, 1>() = -tmp.transpose() * position;
            return r;
        }

        static Eigen::Matrix4f view(const Eigen::Vector3f& location, const Eigen::Vector3f& forward, const Eigen::Vector3f& up)
        {
            return Eigen::Affine3f(Eigen::Translation3f(location)).matrix();

            // TODO
            //Eigen::Vector3f transformedForward = forward; //rotation.transformVector(forward, forward);
            //Eigen::Vector3f cameraLookAt = location + transformedForward;
            //return lookAt(location, cameraLookAt, Eigen::Vector3f(0.0f, 1.0f, 0.0f));
        }

        static Eigen::Matrix4f perspectiveProjection(float fovY, float aspect, float near, float far)
        {
            float theta = fovY * 0.5f;
            float range = far - near;
            float invtan = 1.0f / tan(theta);

            Eigen::Matrix4f r;
            r(0, 0) = invtan / aspect;
            r(1, 1) = invtan;
            r(2, 2) = -(near + far) / range;
            r(3, 2) = -1;
            r(2, 3) = -2 * near * far / range;
            r(3, 3) = 0;
            return r;
        }

        static Eigen::Matrix4f scale(float x, float y, float z)
        {
            Eigen::Matrix4f r = Eigen::Matrix4f::Identity();
            r(0, 0) = x;
            r(1, 1) = y;
            r(2, 2) = z;
            return r;
        }

        static Eigen::Matrix4f orthogonalProjection(float w, float h, float near, float far)
        {
            Eigen::Matrix4f r = Eigen::Matrix4f::Identity();

            r(0, 0) = 2.0f / w;
            r(1, 1) = 2.0f / h;
            r(2, 2) = 1.0f / (far - near);
            r(2, 3) = -near / (far - near);
            r(3, 3) = 1.0f;
            return r;
        }

        /**
         * Returns next power of two size for given number.
         */
        static unsigned int nextPowerOfTwo(unsigned int n)
        {
            n--;
            n |= n >> 1;
            n |= n >> 2;
            n |= n >> 4;
            n |= n >> 8;
            n |= n >> 16;
            n++;
            return n;
        }

        static float cross(const Eigen::Vector2f & a, const Eigen::Vector2f & b)
        {
            return a.x() * b.y() - a.y() * b.x();
        }

        static Eigen::Vector2f normal(const Eigen::Vector2f & line)
        {
            return Eigen::Vector2f(line.y(), -line.x());
        }

        /**
         * Returns minimum distance between line segment vw and point p.
         * returns the closest point on the line segment.
         */
        static float pointDistanceToLine(const Eigen::Vector2f & pt, const Eigen::Vector2f & p1, const Eigen::Vector2f & p2, Eigen::Vector2f & closest)
        {
            float dx = p2.x() - p1.x();
            float dy = p2.y() - p1.y();
            if((dx == 0) && (dy == 0))
            {
                // Line is a point!
                closest = p1;
                dx = pt.x() - p1.x();
                dy = pt.y() - p1.y();
            }
            else
            {
                // Calculate the t that minimizes the distance
                float t = ((pt.x() - p1.x()) * dx + (pt.y() - p1.y()) * dy) / (dx * dx + dy * dy);

                // See if this represents one of the segment's  end points or
                // a point in the middle.
                if(t < 0)
                {
                    closest = Eigen::Vector2f(p1.x(), p1.y());
                    dx = pt.x() - p1.x();
                    dy = pt.y() - p1.y();
                }
                else if(t > 1)
                {
                    closest = Eigen::Vector2f(p2.x(), p2.y());
                    dx = pt.x() - p2.x();
                    dy = pt.y() - p2.y();
                }
                else
                {
                    closest = Eigen::Vector2f(p1.x() + t * dx, p1.y() + t * dy);
                    dx = pt.x() - closest.x();
                    dy = pt.y() - closest.y();
                }
            }

            return sqrt(dx * dx + dy * dy);
        }
    }
}

#endif
