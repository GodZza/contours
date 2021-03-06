//
//  Filename         : GeomUtils.h
//  Author(s)        : Stephane Grabli
//  Purpose          : Various tools for geometry
//  Date of creation : 12/04/2002
//
///////////////////////////////////////////////////////////////////////////////


//
//  Copyright (C) : Please refer to the COPYRIGHT file distributed 
//   with this source distribution. 
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef  GEOMUTILS_H
# define GEOMUTILS_H

# include <vector>
# include "../system/FreestyleConfig.h"
# include "Geom.h"

using namespace std;
using namespace Geometry;

namespace GeomUtils {

  //
  // Templated procedures
  //
  /////////////////////////////////////////////////////////////////////////////

  /*! Computes the distance from a point P to a segment AB */
  template<class T>
  real distPointSegment( const T& P, const T& A , const T& B) { 
    T AB, AP, BP;
    AB = B - A;
    AP = P - A;
    BP = P - B;

    real c1(AB * AP);
    if (c1 <= 0)
      return AP.norm();

    real c2(AB * AB);
    if (c2 <= c1)
      return BP.norm();

    real b = c1 / c2;
    T Pb, PPb;
    Pb = A + b * AB;
    PPb = P - Pb;

    return PPb.norm();
  } 

  //
  // Non-templated procedures
  //
  /////////////////////////////////////////////////////////////////////////////

  typedef enum {
    DONT_INTERSECT,
    DO_INTERSECT,
    COLINEAR,
    COINCIDENT
  } intersection_test;

  LIB_GEOMETRY_EXPORT
  intersection_test intersect2dSeg2dSeg(const Vec2r& p1, const Vec2r& p2, // first segment
					const Vec2r& p3, const Vec2r& p4, // second segment
					Vec2r& res);                      // found intersection point

  LIB_GEOMETRY_EXPORT
  intersection_test intersect2dLine2dLine(const Vec2r& p1, const Vec2r& p2, // first segment
					  const Vec2r& p3, const Vec2r& p4, // second segment
					  Vec2r& res);                      // found intersection point

  LIB_GEOMETRY_EXPORT
  intersection_test intersect2dSeg2dSegParametric(const Vec2r& p1, const Vec2r& p2, // first segment
						  const Vec2r& p3, const Vec2r& p4, // second segment
						  real& t,                          // I = P1 + t * P1P2)
						  real& u);                         // I = P3 + u * P3P4

  /*! check whether a 2D segment intersect a 2D region or not */
  LIB_GEOMETRY_EXPORT
  bool intersect2dSeg2dArea(const Vec2r& min,
			    const Vec2r& max,
			    const Vec2r& A,
			    const Vec2r& B);
  
  /*! check whether a 2D segment is included in a 2D region or not */
  LIB_GEOMETRY_EXPORT
  bool include2dSeg2dArea(const Vec2r& min,
			  const Vec2r& max,
			  const Vec2r& A,
			  const Vec2r& B);

  /*! Box-triangle overlap test, adapted from Tomas Akenine-M�ller code */
  LIB_GEOMETRY_EXPORT
  bool overlapTriangleBox(Vec3r& boxcenter,
			  Vec3r& boxhalfsize,
			  Vec3r triverts[3]);

  /*! Fast, Minimum Storage Ray-Triangle Intersection,
   * adapted from Tomas M�ller and Ben Trumbore code.
   */
  LIB_GEOMETRY_EXPORT
  bool intersectRayTriangle(Vec3r& orig, Vec3r& dir,
			    Vec3r& v0, Vec3r& v1, Vec3r& v2,
			    real& t,                   // I = orig + t * dir
                real& u, real& v,          // I = (1-u-v)*v0+u*v1+v*v2
			    real epsilon = M_EPSILON); // the epsilon to use

  /*! Intersection between plane and ray
   * adapted from Graphics Gems, Didier Badouel 
   */
  LIB_GEOMETRY_EXPORT
  intersection_test intersectRayPlane(Vec3r& orig, Vec3r& dir,   // ray origin and direction
				      Vec3r& norm, real d,       // plane's normal and offset (plane = { P / P.N + d = 0 })
				      real& t,                   // I = orig + t * dir
				      real epsilon = M_EPSILON); // the epsilon to use

  LIB_GEOMETRY_EXPORT
  intersection_test intersectRayPlanePN(Vec3r& orig, Vec3r& dir,   // ray origin and direction
				      Vec3r& norm, Vec3r &ptOnPlane,       // plane's normal and a point on the plane
				      real& t,                   // I = orig + t * dir
				      real epsilon = M_EPSILON); // the epsilon to use

  LIB_GEOMETRY_EXPORT
  intersection_test intersectLinePlanePN(Vec3r& orig, Vec3r& dir,   // ray origin and direction
					 Vec3r& norm, Vec3r &ptOnPlane,       // plane's normal and a point on the plane
					 real& t,                   // I = orig + t * dir
					 real epsilon = M_EPSILON); // the epsilon to use

 /*! Intersection Ray-Bounding box (axis aligned).
  *  Adapted from Williams et al, "An Efficient Robust Ray-Box Intersection Algorithm", 
  *  JGT 10:1 (2005), pp. 49-54.
  *  Returns
  */
  LIB_GEOMETRY_EXPORT
      bool intersectRayBBox(const Vec3r& orig, const Vec3r& dir,   // ray origin and direction
      const Vec3r& boxMin, const Vec3r& boxMax, // the bbox
      real t0, real t1,     // the interval in which at least on of the intersections must happen
      real& tmin, real& tmax,                  // Imin=orig+tmin*dir is the first intersection, Imax=orig+tmax*dir is the second intersection
      real epsilon = M_EPSILON); // the epsilon to use


  /*! Checks whether 3D point P lies inside or outside of the triangle ABC */
  LIB_GEOMETRY_EXPORT
  bool includePointTriangle(Vec3r& P,
			    Vec3r& A,
			    Vec3r& B,
			    Vec3r& C);

  LIB_GEOMETRY_EXPORT
  void transformVertex(const Vec3r& vert,
		       const Matrix44r& matrix,
		       Vec3r& res);
  
  LIB_GEOMETRY_EXPORT
  void transformVertices(const vector<Vec3r>& vertices,
			 const Matrix44r& trans,
			 vector<Vec3r>& res);

  LIB_GEOMETRY_EXPORT
  Vec3r rotateVector(const Matrix44r& mat, const Vec3r& v);

  //
  // Coordinates systems changing procedures
  //
  /////////////////////////////////////////////////////////////////////////////

  /*! From world to image
   *  p
   *    point's coordinates expressed in world coordinates system
   *  q
   *    vector in which the result will be stored
   *  model_view_matrix
   *    The model view matrix expressed in line major order (OpenGL
   *    matrices are column major ordered)
   *  projection_matrix
   *    The projection matrix expressed in line major order (OpenGL
   *    matrices are column major ordered)
   *  viewport
   *    The viewport: x,y coordinates followed by width and height (OpenGL like viewport)
   */
  LIB_GEOMETRY_EXPORT
  void fromWorldToImage(const Vec3r& p,
			Vec3r& q,
			const real model_view_matrix[4][4],
			const real projection_matrix[4][4],
			const int viewport[4]);

  /*! From world to image
   *  p
   *    point's coordinates expressed in world coordinates system
   *  q
   *    vector in which the result will be stored
   *  transform
   *    The transformation matrix (gathering model view and projection), 
   *    expressed in line major order (OpenGL matrices are column major ordered)
   *  viewport
   *    The viewport: x,y coordinates followed by width and height (OpenGL like viewport)
   */
  LIB_GEOMETRY_EXPORT
  void fromWorldToImage(const Vec3r& p,
			Vec3r& q,
			const real transform[4][4],
			const int viewport[4]);

  /*! Projects from world coordinates to camera coordinates 
   *  Returns the point's coordinates expressed in the camera's 
   *  coordinates system.
   *  p
   *    point's coordinates expressed in world coordinates system
   *  q
   *    vector in which the result will be stored
   *  model_view_matrix
   *    The model view matrix expressed in line major order (OpenGL
   *    matrices are column major ordered)
   */
  LIB_GEOMETRY_EXPORT
  void fromWorldToCamera(const Vec3r& p,
			 Vec3r& q,
			 const real model_view_matrix[4][4]);
  
  /*! Projects from World Coordinates to retina coordinates
   *  Returns the point's coordinates expressed in Retina system.
   *  p
   *    point's coordinates expressed in camera system
   *  q
   *    vector in which the result will be stored
   *  projection_matrix
   *    The projection matrix expressed in line major order (OpenGL
   *    matrices are column major ordered)
   */
  LIB_GEOMETRY_EXPORT
  void fromCameraToRetina(const Vec3r& p,
			  Vec3r& q,
			  const real projection_matrix[4][4]);
  
  /*! From retina to image.
   *  Returns the coordinates expressed in Image coorinates system.
   *  p
   *    point's coordinates expressed in retina system
   *  q
   *    vector in which the result will be stored
   *  viewport
   *    The viewport: x,y coordinates followed by width and height (OpenGL like viewport).
   */
  LIB_GEOMETRY_EXPORT
  void fromRetinaToImage(const Vec3r& p,
			 Vec3r& q,
			 const int viewport[4]);
  
  /*! From image to retina
   *  p
   *    point's coordinates expressed in image system
   *  q
   *    vector in which the result will be stored
   *  viewport
   *    The viewport: x,y coordinates followed by width and height (OpenGL like viewport).
   */
  LIB_GEOMETRY_EXPORT
  void fromImageToRetina(const Vec3r& p,
			 Vec3r& q,
			 const int viewport[4]);
 
  /*! computes the coordinates of q in the camera coordinates system, 
   *  using the known z coordinates of the 3D point. 
   *  That means that this method does not inverse any matrices, 
   *  it only computes X and Y from x,y and Z)
   *  p
   *    point's coordinates expressed in retina system
   *  q
   *    vector in which the result will be stored
   *  projection_matrix
   *    The projection matrix expressed in line major order (OpenGL
   *    matrices are column major ordered)
   
   */
  LIB_GEOMETRY_EXPORT
  void fromRetinaToCamera(const Vec3r& p,
			  Vec3r& q,
			  real z,
			  const real projection_matrix[4][4]);

  /*! Projects from camera coordinates to world coordinates
   *  Returns the point's coordinates expressed in the world's 
   *  coordinates system.
   *  p
   *    point's coordinates expressed in the camera coordinates system
   *  q
   *    vector in which the result will be stored
   *  model_view_matrix
   *    The model view matrix expressed in line major order (OpenGL
   *    matrices are column major ordered)
   */
  LIB_GEOMETRY_EXPORT
  void fromCameraToWorld(const Vec3r& p,
			 Vec3r& q,
			 const real model_view_matrix[4][4]);


  LIB_GEOMETRY_EXPORT
  Vec3r ComputeBarycentricCoords(const Vec3r A,const Vec3r B,const Vec3r C,const Vec3r P);
  
  LIB_GEOMETRY_EXPORT inline
  Vec3r Bary2Point(const Vec3r A,const Vec3r B,const Vec3r C,const Vec3r P)
  { return A*P.x() + B*P.y() + C*P.z(); }

  LIB_GEOMETRY_EXPORT
  inline
  real segmentParam(const Vec2r A, const Vec2r B, const Vec2r P)
  {
    Vec2r PA = P-A;
    Vec2r BA = B-A;
    return (PA * BA) / (BA*BA);
  }

  LIB_GEOMETRY_EXPORT
  inline
  real segmentParam(const Vec3r A, const Vec3r B, const Vec3r P)
  {
    Vec3r PA = P-A;
    Vec3r BA = B-A;
    return (PA * BA) / (BA*BA);
  }

  LIB_GEOMETRY_EXPORT
  bool closestPoints3dLines(const Vec3r p1, const Vec3r dir1, const Vec3r p2, const Vec3r dir2,
			   Vec3r & pt1, Vec3r & pt2);


  LIB_GEOMETRY_EXPORT
  intersection_test intersect3dLines(const Vec3r p1, const Vec3r p2, const Vec3r q1, const Vec3r q2, Vec3r & pt);


  //  LIB_GEOMETRY_EXPORT
  //  intersection_test intersect3dLineSegments(const Vec3r p1, const Vec3r p2, const Vec3r q1, const Vec3r q2, Vec3r & pt);

} // end of namespace GeomUtils

#endif // GEOMUTILS_H
