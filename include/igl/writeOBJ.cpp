// This file is part of libigl, a simple c++ geometry processing library.
//
// Copyright (C) 2013 Alec Jacobson <alecjacobson@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla Public License
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at http://mozilla.org/MPL/2.0/.
#include "writeOBJ.h"

#include <iostream>
#include <limits>
#include <iomanip>
#include <fstream>
#include <cstdio>
#include <cassert>

template <typename DerivedV, typename DerivedF, typename DerivedT>
IGL_INLINE bool igl::writeOBJ(
  const std::string str,
  const Eigen::PlainObjectBase<DerivedV>& V,
  const Eigen::PlainObjectBase<DerivedF>& F,
  const Eigen::PlainObjectBase<DerivedV>& CN,
  const Eigen::PlainObjectBase<DerivedF>& FN,
  const Eigen::PlainObjectBase<DerivedT>& TC,
  const Eigen::PlainObjectBase<DerivedF>& FTC)
{
  FILE * obj_file = fopen(str.c_str(),"w");
  if(NULL==obj_file)
  {
    printf("IOError: %s could not be opened for writing...",str.c_str());
    return false;
  }
  // Loop over V
  for(int i = 0;i<(int)V.rows();i++)
  {
    fprintf(obj_file,"v %0.17g %0.17g %0.17g\n",
      V(i,0),
      V(i,1),
      V(i,2)
      );
  }
  bool write_N = CN.rows() >0;

  if(write_N)
  {
    for(int i = 0;i<(int)CN.rows();i++)
    {
      fprintf(obj_file,"vn %0.17g %0.17g %0.17g\n",
              CN(i,0),
              CN(i,1),
              CN(i,2)
              );
    }
    fprintf(obj_file,"\n");
  }

  bool write_texture_coords = TC.rows() >0;

  if(write_texture_coords)
  {
    for(int i = 0;i<(int)TC.rows();i++)
    {
      fprintf(obj_file, "vt %0.17g %0.17g\n",TC(i,0),TC(i,1));
    }
    fprintf(obj_file,"\n");
  }

  // loop over F
  for(int i = 0;i<(int)F.rows();++i)
  {
    fprintf(obj_file,"f");
    for(int j = 0; j<(int)F.cols();++j)
    {
      // OBJ is 1-indexed
      fprintf(obj_file," %u",F(i,j)+1);

      if(write_texture_coords)
        fprintf(obj_file,"/%u",FTC(i,j)+1);
      if(write_N)
      {
        if (write_texture_coords)
          fprintf(obj_file,"/%u",FN(i,j)+1);
        else
          fprintf(obj_file,"//%u",FN(i,j)+1);
      }
    }
    fprintf(obj_file,"\n");
  }
  fclose(obj_file);
  return true;
}

template <typename DerivedV, typename DerivedF>
IGL_INLINE bool igl::writeOBJ(
  const std::string str,
  const Eigen::PlainObjectBase<DerivedV>& V,
  const Eigen::PlainObjectBase<DerivedF>& F)
{
  using namespace std;
  using namespace Eigen;
  assert(V.cols() == 3 && "V should have 3 columns");
  ofstream s(str);
  if(!s.is_open())
  {
    fprintf(stderr,"IOError: writeOBJ() could not open %s\n",str.c_str());
    return false;
  }
  s<<
    V.format(IOFormat(FullPrecision,DontAlignCols," ","\n","v ","","","\n"))<<
    (F.array()+1).format(IOFormat(FullPrecision,DontAlignCols," ","\n","f ","","","\n"));
  return true;
}

#ifdef IGL_STATIC_LIBRARY
// Explicit template specialization
// generated by autoexplicit.sh
template bool igl::writeOBJ<Eigen::Matrix<double, 8, 3, 0, 8, 3>, Eigen::Matrix<int, 12, 3, 0, 12, 3> >(std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Eigen::PlainObjectBase<Eigen::Matrix<double, 8, 3, 0, 8, 3> > const&, Eigen::PlainObjectBase<Eigen::Matrix<int, 12, 3, 0, 12, 3> > const&);
// generated by autoexplicit.sh
template bool igl::writeOBJ<Eigen::Matrix<double, -1, -1, 0, -1, -1>, Eigen::Matrix<int, -1, -1, 0, -1, -1> >(std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&);
template bool igl::writeOBJ<Eigen::Matrix<double, -1, 3, 1, -1, 3>, Eigen::Matrix<unsigned int, -1, -1, 1, -1, -1>, Eigen::Matrix<double, -1, 2, 1, -1, 2> >(std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 3, 1, -1, 3> > const&, Eigen::PlainObjectBase<Eigen::Matrix<unsigned int, -1, -1, 1, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 3, 1, -1, 3> > const&, Eigen::PlainObjectBase<Eigen::Matrix<unsigned int, -1, -1, 1, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 2, 1, -1, 2> > const&, Eigen::PlainObjectBase<Eigen::Matrix<unsigned int, -1, -1, 1, -1, -1> > const&);
template bool igl::writeOBJ<Eigen::Matrix<float, -1, 3, 1, -1, 3>, Eigen::Matrix<unsigned int, -1, -1, 1, -1, -1>, Eigen::Matrix<float, -1, 2, 1, -1, 2> >(std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Eigen::PlainObjectBase<Eigen::Matrix<float, -1, 3, 1, -1, 3> > const&, Eigen::PlainObjectBase<Eigen::Matrix<unsigned int, -1, -1, 1, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<float, -1, 3, 1, -1, 3> > const&, Eigen::PlainObjectBase<Eigen::Matrix<unsigned int, -1, -1, 1, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<float, -1, 2, 1, -1, 2> > const&, Eigen::PlainObjectBase<Eigen::Matrix<unsigned int, -1, -1, 1, -1, -1> > const&);
template bool igl::writeOBJ<Eigen::Matrix<double, -1, -1, 0, -1, -1>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, Eigen::Matrix<double, -1, -1, 0, -1, -1> >(std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&);
template bool igl::writeOBJ<Eigen::Matrix<double, -1, -1, 0, -1, -1>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, Eigen::Matrix<double, -1, 2, 0, -1, 2> >(std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 2, 0, -1, 2> > const&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&);
template bool igl::writeOBJ<Eigen::Matrix<double, -1, 3, 0, -1, 3>, Eigen::Matrix<int, -1, 3, 0, -1, 3> >(std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 3, 0, -1, 3> > const&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, 3, 0, -1, 3> > const&);
template bool igl::writeOBJ<Eigen::Matrix<double, -1, 3, 0, -1, 3>, Eigen::Matrix<int, -1, 3, 0, -1, 3>, Eigen::Matrix<double, -1, -1, 0, -1, -1> >(std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 3, 0, -1, 3> > const&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, 3, 0, -1, 3> > const&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 3, 0, -1, 3> > const&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, 3, 0, -1, 3> > const&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, 3, 0, -1, 3> > const&);
template bool igl::writeOBJ<Eigen::Matrix<double, -1, 3, 1, -1, 3>, Eigen::Matrix<int, -1, 3, 1, -1, 3> >(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 3, 1, -1, 3> > const&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, 3, 1, -1, 3> > const&);
#endif
