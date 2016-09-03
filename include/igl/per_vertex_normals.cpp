// This file is part of libigl, a simple c++ geometry processing library.
//
// Copyright (C) 2013 Alec Jacobson <alecjacobson@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla Public License
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at http://mozilla.org/MPL/2.0/.
#include "per_vertex_normals.h"

#include "get_seconds.h"
#include "per_face_normals.h"
#include "doublearea.h"
#include "parallel_for.h"
#include "internal_angles.h"

template <typename DerivedV, typename DerivedF>
IGL_INLINE void igl::per_vertex_normals(
  const Eigen::PlainObjectBase<DerivedV>& V,
  const Eigen::PlainObjectBase<DerivedF>& F,
  const igl::PerVertexNormalsWeightingType weighting,
  Eigen::PlainObjectBase<DerivedV> & N)
{
  Eigen::Matrix<typename DerivedV::Scalar,Eigen::Dynamic,3> PFN;
  igl::per_face_normals(V,F,PFN);
  return per_vertex_normals(V,F,weighting,PFN,N);
}

template <typename DerivedV, typename DerivedF>
IGL_INLINE void igl::per_vertex_normals(
  const Eigen::PlainObjectBase<DerivedV>& V,
  const Eigen::PlainObjectBase<DerivedF>& F,
  Eigen::PlainObjectBase<DerivedV> & N)
{
  return per_vertex_normals(V,F,PER_VERTEX_NORMALS_WEIGHTING_TYPE_DEFAULT,N);
}

template <typename DerivedV, typename DerivedF, typename DerivedFN, typename DerivedN>
IGL_INLINE void igl::per_vertex_normals(
  const Eigen::PlainObjectBase<DerivedV>& V,
  const Eigen::PlainObjectBase<DerivedF>& F,
  const igl::PerVertexNormalsWeightingType weighting,
  const Eigen::PlainObjectBase<DerivedFN>& FN,
  Eigen::PlainObjectBase<DerivedN> & N)
{
  using namespace std;
  // Resize for output
  N.setZero(V.rows(),3);

  Eigen::Matrix<typename DerivedN::Scalar,DerivedF::RowsAtCompileTime,3>
    W(F.rows(),3);
  switch(weighting)
  {
    case PER_VERTEX_NORMALS_WEIGHTING_TYPE_UNIFORM:
      W.setConstant(1.);
      break;
    default:
      assert(false && "Unknown weighting type");
    case PER_VERTEX_NORMALS_WEIGHTING_TYPE_DEFAULT:
    case PER_VERTEX_NORMALS_WEIGHTING_TYPE_AREA:
    {
      Eigen::Matrix<typename DerivedN::Scalar,DerivedF::RowsAtCompileTime,1> A;
      doublearea(V,F,A);
      W = A.replicate(1,3);
      break;
    }
    case PER_VERTEX_NORMALS_WEIGHTING_TYPE_ANGLE:
      internal_angles(V,F,W);
      break;
  }

  // loop over faces
  for(int i = 0;i<F.rows();i++)
  {
    // throw normal at each corner
    for(int j = 0; j < 3;j++)
    {
      N.row(F(i,j)) += W(i,j) * FN.row(i);
    }
  }

  //// loop over faces
  //std::mutex critical;
  //std::vector<DerivedN> NN;
  //parallel_for(
  //  F.rows(),
  //  [&NN,&N](const size_t n){ NN.resize(n,DerivedN::Zero(N.rows(),3));},
  //  [&F,&W,&FN,&NN,&critical](const int i, const size_t t)
  //  {
  //    // throw normal at each corner
  //    for(int j = 0; j < 3;j++)
  //    {
  //      // Q: Does this need to be critical?
  //      // A: Yes. Different (i,j)'s could produce the same F(i,j)
  //      NN[t].row(F(i,j)) += W(i,j) * FN.row(i);
  //    }
  //  },
  //  [&N,&NN](const size_t t){ N += NN[t]; },
  //  1000l);

  // take average via normalization
  N.rowwise().normalize();
}

template <typename DerivedV, typename DerivedF>
IGL_INLINE void igl::per_vertex_normals(
  const Eigen::PlainObjectBase<DerivedV>& V,
  const Eigen::PlainObjectBase<DerivedF>& F,
  const Eigen::PlainObjectBase<DerivedV>& FN,
  Eigen::PlainObjectBase<DerivedV> & N)
{
  return
    per_vertex_normals(V,F,PER_VERTEX_NORMALS_WEIGHTING_TYPE_DEFAULT,FN,N);
}

#ifdef IGL_STATIC_LIBRARY
// Explicit template specialization
template void igl::per_vertex_normals<Eigen::Matrix<double, -1, -1, 0, -1, -1>, Eigen::Matrix<int, -1, -1, 0, -1, -1> >(Eigen::PlainObjectBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> >&);
template void igl::per_vertex_normals<Eigen::Matrix<double, -1, -1, 0, -1, -1>, Eigen::Matrix<int, -1, -1, 0, -1, -1> >(Eigen::PlainObjectBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> >&);
template void igl::per_vertex_normals<Eigen::Matrix<double, -1, 3, 0, -1, 3>, Eigen::Matrix<int, -1, 3, 0, -1, 3>, Eigen::Matrix<double, -1, 3, 0, -1, 3>, Eigen::Matrix<double, -1, 3, 0, -1, 3> >(Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 3, 0, -1, 3> > const&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, 3, 0, -1, 3> > const&, igl::PerVertexNormalsWeightingType, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 3, 0, -1, 3> > const&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 3, 0, -1, 3> >&);
template void igl::per_vertex_normals<Eigen::Matrix<double, -1, 3, 0, -1, 3>, Eigen::Matrix<int, -1, 3, 0, -1, 3> >(Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 3, 0, -1, 3> > const&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, 3, 0, -1, 3> > const&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 3, 0, -1, 3> >&);
template void igl::per_vertex_normals<Eigen::Matrix<double, -1, 3, 0, -1, 3>, Eigen::Matrix<int, -1, 3, 0, -1, 3> >(Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 3, 0, -1, 3> > const&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, 3, 0, -1, 3> > const&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 3, 0, -1, 3> > const&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 3, 0, -1, 3> >&);
template void igl::per_vertex_normals<Eigen::Matrix<float, -1, 3, 1, -1, 3>, Eigen::Matrix<unsigned int, -1, 3, 1, -1, 3> >(Eigen::PlainObjectBase<Eigen::Matrix<float, -1, 3, 1, -1, 3> > const&, Eigen::PlainObjectBase<Eigen::Matrix<unsigned int, -1, 3, 1, -1, 3> > const&, Eigen::PlainObjectBase<Eigen::Matrix<float, -1, 3, 1, -1, 3> >&);
template void igl::per_vertex_normals<Eigen::Matrix<double, -1, -1, 0, -1, -1>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, Eigen::Matrix<double, -1, -1, 0, -1, -1>, Eigen::Matrix<double, -1, -1, 0, -1, -1> >(Eigen::PlainObjectBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, igl::PerVertexNormalsWeightingType, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> >&);
template void igl::per_vertex_normals<Eigen::Matrix<double, -1, -1, 0, -1, -1>, Eigen::Matrix<int, -1, -1, 0, -1, -1> >(Eigen::PlainObjectBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, igl::PerVertexNormalsWeightingType, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> >&);
template void igl::per_vertex_normals<Eigen::Matrix<double, -1, 3, 1, -1, 3>, Eigen::Matrix<int, -1, 3, 1, -1, 3> >(Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 3, 1, -1, 3> > const&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, 3, 1, -1, 3> > const&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 3, 1, -1, 3> >&);
#endif
