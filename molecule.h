
/****************************************************************************
**
** Copyright (C) 2011 Christian B. Huebschle & George M. Sheldrick
** All rights reserved.
** Contact: chuebsch@moliso.de
**
** This file is part of the ShelXle
**
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file COPYING included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
**
****************************************************************************/
#ifndef MOLECULE_H
#define MOLECULE_H 1
#include <math.h>
#ifndef fmin
#define fmin(x, y) (((x) < (y)) ? (x) : (y))
#endif
#ifndef round
#define round(x) (x<0?ceil((x)-0.5):floor((x)+0.5))
#endif
#include <QString>
#include <QStringList>
#include <QList>
#include <QtOpenGL>
#include <QtCore>
#ifndef M_PI
#define	M_PI		3.14159265358979323846
#endif
#define ATOM_STYLE_WALLS 1
#define ATOM_STYLE_RINGS 2
#define ATOM_STYLE_SPHERE 4
#define ATOM_STYLE_SOLID 8
#define ATOM_STYLE_WHITERING 16
#define ATOM_STYLE_NOLABEL 32
#define ATOM_STYLE_NOADP 64
#define ATOM_STYLE_PLAID 128
#define ATOM_STYLE_METAL 256
#include <QSettings>

//GLOBAL SETTINGS
//static int global_zero_counter=0;
//! V3 is a three dimensional vector in cartesian space
struct V3 {
  double x//! x is the X coordinate 
	 ,y//! y is the Y coordinate 
	 ,z//! z is the Z coordinate
	 ;
//  int rc;
  inline V3( void ){}
  inline V3( const double& _x, const double& _y, const double& _z ) : 
  x(_x), y(_y), z(_z)//!< initializer
       	//,rc(0) 
  {
    ;
  }
  inline V3& operator *= ( const double& d ){
    x *= d;
    y *= d;
    z *= d;
    return *this;
  }//!< The *= operator to scale by a scalar
  inline V3& operator += ( const V3& v ){
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }//!< The += operator to add a V3  
  inline V3& operator += ( const double& v ){
    x += v;
    y += v;
    z += v;
    return *this;
  }//!< The += operator to add a scalar
};
inline V3 operator + ( const V3& v1, const V3& v2 ) {
  V3 t;
  t.x = v1.x + v2.x;
  t.y = v1.y + v2.y;
  t.z = v1.z + v2.z;
  return t;
}//!< The + operator to add two V3
inline V3 operator - ( const V3& v1, const V3& v2 ) {
  V3 t;
  t.x = v1.x - v2.x;
  t.y = v1.y - v2.y;
  t.z = v1.z - v2.z;
  return t;
}//!< The + operator to subtract two V3
inline V3 operator * ( const V3& v, const double& d ) {
  V3 t;
  t.x = v.x*d;
  t.y = v.y*d;
  t.z = v.z*d;
  return t;
}//!< The * to scale a V3
inline V3 operator * ( const double& d, const V3& v ) {
  V3 t;
  t.x = v.x*d;
  t.y = v.y*d;
  t.z = v.z*d;
  return t;
}//!< The * to scale a V3
inline V3 operator % ( const V3& v1, const V3& v2 ) {
  V3 t;
  t.x = v1.y*v2.z - v2.y*v1.z;
  t.y = v1.z*v2.x - v2.z*v1.x;
  t.z = v1.x*v2.y - v2.x*v1.y;
  return t;
}//!< The % operator the cross product of two V3
inline double operator * ( const V3& v1, const V3& v2 ) {
  return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}//!< The * operator the scalar product of two V3
inline double Norm( const V3& v ) {
  return v.x*v.x + v.y*v.y + v.z*v.z;
}//!< The squared lenght of a V3
inline double Distance( const V3& v1, const V3& v2 ) {
  return Norm(v1 - v2);
}//!< The squared distance between two V3
inline bool operator == (const V3& v1, const V3& v2 ) {
  //  return ((v1.x==v2.x)&&(v1.y==v2.y)&&(v1.z==v2.z));
  return (Distance(v1,v2)<0.001);
}
inline V3& Normalize( V3 v ) {
static V3 erg=V3(1,0,0);
  if (Norm(v))  erg= (v * (1.0/sqrt(Norm(v)))); 
  return erg; 
}
//! Matrix is a 3 x 3 Matrix with all needed operators
struct Matrix{
double m11, m21, m31, m12, m22, m32, m13, m23, m33;
 inline Matrix(void){}
 inline Matrix( const V3 &a, const V3 &b, const V3 &c):
	 m11(a.x), m21(b.x), m31(c.x),
	 m12(a.y), m22(b.y), m32(c.y),
	 m13(a.z), m23(b.z), m33(c.z){;}
 inline Matrix( const double& x11, const double& x21, const double& x31,
                const double& x12, const double& x22, const double& x32,
                const double& x13, const double& x23, const double& x33):
	 m11(x11), m21(x21), m31(x31),
	 m12(x12), m22(x22), m32(x32),
	 m13(x13), m23(x23), m33(x33){;}

};
 inline Matrix transponse (Matrix a){//transponse
    return Matrix(
		  a.m11, a.m12, a.m13,
		  a.m21, a.m22, a.m23,
		  a.m31, a.m32, a.m33);
 }
 inline bool operator == (const Matrix &a,const Matrix &b){
     return ((a.m11 == b.m11)&&(a.m21 == b.m21)&&(a.m31 == b.m31)&&
     (a.m12 == b.m12)&&(a.m22 == b.m22)&&(a.m23 == b.m23)&&
     (a.m13 == b.m13)&&(a.m32 == b.m32)&&(a.m33 == b.m33));
 }
inline Matrix operator * (const Matrix &a,const Matrix &b){
  Matrix erg;
  erg.m11 = a.m11 * b.m11 + a.m21 * b.m12 + a.m31 * b.m13;
  erg.m21 = a.m11 * b.m21 + a.m21 * b.m22 + a.m31 * b.m23;
  erg.m31 = a.m11 * b.m31 + a.m21 * b.m32 + a.m31 * b.m33;

  erg.m12 = a.m12 * b.m11 + a.m22 * b.m12 + a.m32 * b.m13;
  erg.m22 = a.m12 * b.m21 + a.m22 * b.m22 + a.m32 * b.m23;
  erg.m32 = a.m12 * b.m31 + a.m22 * b.m32 + a.m32 * b.m33;

  erg.m13 = a.m13 * b.m11 + a.m23 * b.m12 + a.m33 * b.m13;
  erg.m23 = a.m13 * b.m21 + a.m23 * b.m22 + a.m33 * b.m23;
  erg.m33 = a.m13 * b.m31 + a.m23 * b.m32 + a.m33 * b.m33;
  return erg;
}
inline V3 operator * (const Matrix &a, const V3 &b){
  V3 erg;
  erg.x = a.m11*b.x + a.m21*b.y + a.m31*b.z;
  erg.y = a.m12*b.x + a.m22*b.y + a.m32*b.z;
  erg.z = a.m13*b.x + a.m23*b.y + a.m33*b.z;
  return erg;
}
inline V3 operator * (const V3 &a, const Matrix &b){
  V3 erg;
  erg.x = b.m11*a.x + b.m12*a.y + b.m13*a.z;
  erg.y = b.m21*a.x + b.m22*a.y + b.m23*a.z;
  erg.z = b.m31*a.x + b.m32*a.y + b.m33*a.z;
  return erg;
}
//! MyAtom an atom object
typedef struct MyAtom{
  //! Label is the atom label with residue nuber seperated by '_' and the symmetry operation number seperated by french quotes.
  QString Label;
  //! ResiClass is the four or three letters residue class
  QString ResiClass;
  //! The origanal line in the res file.
  QString orginalLine;
  //! fragment number of the structure part
  int molindex;
  int fixFlag;
  //! the symmetry number  
  int symmGroup ;
  //! a is hidden flag
  int hidden;
  //! the site occupation factor as it is the file e.g. 11.000
  double sof_org;
  //! the site occupation factor
  double sof;
  //! the fractional Uij
  Matrix uf;
  //! the cartesian Uij
  Matrix uc;
  //! the cartesian coordinates.
  V3 pos;
  //! the fractional coordinates.
  V3 frac;
  //! the electrondensity value of a Q-Peak.
  double peakHeight;
  //! the residue number
  int resiNr;
  //! the atomic number
  int an;
  //! the (dissordered) part number
  int part;
  //! the style type of the atom
  int style;
  //! the afix type of the parent atom
  int afixParent;
  //! the afix type
  int afix;
  //! the screen position X 
  GLdouble screenX;
  //! the screen position Y 
  GLdouble screenY;
}MyAtom;
inline bool  operator == (const MyAtom &a1,const MyAtom &a2){
  return ((a1.Label == a2.Label)&&(a1.resiNr == a2.resiNr)&&(a1.part == a2.part)&&(a1.symmGroup == a2.symmGroup));
} 
inline bool operator < (const MyAtom &a1, const MyAtom &a2){
  return (a1.Label < a2.Label);
} 
//! MyBond a bond object
typedef struct MyBond{
  //! a pointer to the atom where the bond starts
MyAtom const *ato1;  
 //! a pointer to the atom where the bond ends
MyAtom const *ato2;
//! the bond length
  double length;
  //! the index of the atom where the bond starts  
  int a1;
  //! the index of the atom where the bond ends
  int a2;
  inline MyBond& operator = (const MyBond& b){
    length=b.length;
    ato1=b.ato1;
    ato2=b.ato2;
    a1=b.a1;
    a2=b.a2;
    return *this;
  } 
}MyBond;
//! for the BIND instruction 
typedef struct MyBind{
  QString Lab1,Lab2;
}MyBind;

  const float mCubeVerticesData[1122] = { 
   -0.577350f,  0.577350f,  0.577350f, // 0
    0.577350f,  0.577350f,  0.577350f, // 1
   -0.577350f, -0.577350f,  0.577350f, // 2
    0.577350f, -0.577350f,  0.577350f, // 3
   -0.577350f,  0.577350f, -0.577350f, // 4
    0.577350f,  0.577350f, -0.577350f, // 5
   -0.577350f, -0.577350f, -0.577350f, // 6
    0.577350f, -0.577350f, -0.577350f, // 7
    0.000000f,  1.000000f,  0.000000f, // 8
    1.000000f,  0.000000f,  0.000000f, // 9
    0.000000f, -1.000000f,  0.000000f, // 10
    0.000000f,  0.000000f,  1.000000f, // 11
   -1.000000f,  0.000000f,  0.000000f, // 12
    0.000000f,  0.000000f, -1.000000f, // 13
    0.707107f,  0.707107f,  0.000000f, // 14
    0.000000f,  0.707107f, -0.707107f, // 15
   -0.707107f,  0.707107f,  0.000000f, // 16
    0.000000f,  0.707107f,  0.707107f, // 17
    0.707107f,  0.000000f, -0.707107f, // 18
    0.707107f,  0.707107f,  0.000000f, // 19
    0.707107f,  0.000000f,  0.707107f, // 20
    0.707107f, -0.707107f,  0.000000f, // 21
    0.000000f, -0.707107f, -0.707107f, // 22
    0.707107f, -0.707107f,  0.000000f, // 23
    0.000000f, -0.707107f,  0.707107f, // 24
   -0.707107f, -0.707107f,  0.000000f, // 25
    0.000000f, -0.707107f,  0.707107f, // 26
    0.707107f,  0.000000f,  0.707107f, // 27
    0.000000f,  0.707107f,  0.707107f, // 28
   -0.707107f,  0.000000f,  0.707107f, // 29
   -0.707107f,  0.707107f,  0.000000f, // 30
   -0.707107f,  0.000000f, -0.707107f, // 31
   -0.707107f, -0.707107f,  0.000000f, // 32
   -0.707107f,  0.000000f,  0.707107f, // 33
    0.707107f,  0.000000f, -0.707107f, // 34
    0.000000f, -0.707107f, -0.707107f, // 35
   -0.707107f,  0.000000f, -0.707107f, // 36
    0.000000f,  0.707107f, -0.707107f, // 37
    0.325058f,  0.888074f, -0.325058f, // 38
    0.325058f,  0.888074f,  0.325058f, // 39
   -0.325058f,  0.888074f, -0.325058f, // 40
    0.325058f,  0.888074f, -0.325058f, // 41
   -0.325058f,  0.888074f,  0.325058f, // 42
   -0.325058f,  0.888074f, -0.325058f, // 43
    0.325058f,  0.888074f,  0.325058f, // 44
   -0.325058f,  0.888074f,  0.325058f, // 45
    0.888074f,  0.325058f, -0.325058f, // 46
    0.888074f, -0.325058f, -0.325058f, // 47
    0.888074f,  0.325058f,  0.325058f, // 48
    0.888074f,  0.325058f, -0.325058f, // 49
    0.888074f, -0.325058f,  0.325058f, // 50
    0.888074f,  0.325058f,  0.325058f, // 51
    0.888074f, -0.325058f, -0.325058f, // 52
    0.888074f, -0.325058f,  0.325058f, // 53
    0.325058f, -0.888074f, -0.325058f, // 54
   -0.325058f, -0.888074f, -0.325058f, // 55
    0.325058f, -0.888074f,  0.325058f, // 56
    0.325058f, -0.888074f, -0.325058f, // 57
   -0.325058f, -0.888074f,  0.325058f, // 58
    0.325058f, -0.888074f,  0.325058f, // 59
   -0.325058f, -0.888074f, -0.325058f, // 60
   -0.325058f, -0.888074f,  0.325058f, // 61
    0.325058f, -0.325058f,  0.888074f, // 62
   -0.325058f, -0.325058f,  0.888074f, // 63
    0.325058f,  0.325058f,  0.888074f, // 64
    0.325058f, -0.325058f,  0.888074f, // 65
   -0.325058f,  0.325058f,  0.888074f, // 66
    0.325058f,  0.325058f,  0.888074f, // 67
   -0.325058f, -0.325058f,  0.888074f, // 68
   -0.325058f,  0.325058f,  0.888074f, // 69
   -0.888074f,  0.325058f, -0.325058f, // 70
   -0.888074f,  0.325058f,  0.325058f, // 71
   -0.888074f, -0.325058f, -0.325058f, // 72
   -0.888074f,  0.325058f, -0.325058f, // 73
   -0.888074f, -0.325058f,  0.325058f, // 74
   -0.888074f, -0.325058f, -0.325058f, // 75
   -0.888074f,  0.325058f,  0.325058f, // 76
   -0.888074f, -0.325058f,  0.325058f, // 77
    0.325058f, -0.325058f, -0.888074f, // 78
    0.325058f,  0.325058f, -0.888074f, // 79
   -0.325058f, -0.325058f, -0.888074f, // 80
    0.325058f, -0.325058f, -0.888074f, // 81
   -0.325058f,  0.325058f, -0.888074f, // 82
   -0.325058f, -0.325058f, -0.888074f, // 83
    0.325058f,  0.325058f, -0.888074f, // 84
   -0.325058f,  0.325058f, -0.888074f, // 85
    0.382683f,  0.923880f,  0.000000f, // 86
    0.673887f,  0.673887f, -0.302905f, // 87
    0.673887f,  0.673887f,  0.302905f, // 88
    0.382683f,  0.923880f,  0.000000f, // 89
    0.000000f,  0.923880f, -0.382683f, // 90
   -0.302905f,  0.673887f, -0.673887f, // 91
    0.302905f,  0.673887f, -0.673887f, // 92
    0.000000f,  0.923880f, -0.382683f, // 93
   -0.382683f,  0.923880f,  0.000000f, // 94
   -0.673887f,  0.673887f,  0.302905f, // 95
   -0.673887f,  0.673887f, -0.302905f, // 96
   -0.382683f,  0.923880f,  0.000000f, // 97
    0.000000f,  0.923880f,  0.382683f, // 98
    0.302905f,  0.673887f,  0.673887f, // 99
   -0.302905f,  0.673887f,  0.673887f, // 100
    0.000000f,  0.923880f,  0.382683f, // 101
    0.923880f,  0.000000f, -0.382683f, // 102
    0.673887f,  0.302905f, -0.673887f, // 103
    0.673887f, -0.302905f, -0.673887f, // 104
    0.923880f,  0.000000f, -0.382683f, // 105
    0.923880f,  0.382683f,  0.000000f, // 106
    0.673887f,  0.673887f,  0.302905f, // 107
    0.673887f,  0.673887f, -0.302905f, // 108
    0.923880f,  0.382683f,  0.000000f, // 109
    0.923880f,  0.000000f,  0.382683f, // 110
    0.673887f, -0.302905f,  0.673887f, // 111
    0.673887f,  0.302905f,  0.673887f, // 112
    0.923880f,  0.000000f,  0.382683f, // 113
    0.923880f, -0.382683f,  0.000000f, // 114
    0.673887f, -0.673887f, -0.302905f, // 115
    0.673887f, -0.673887f,  0.302905f, // 116
    0.923880f, -0.382683f,  0.000000f, // 117
    0.000000f, -0.923880f, -0.382683f, // 118
    0.302905f, -0.673887f, -0.673887f, // 119
   -0.302905f, -0.673887f, -0.673887f, // 120
    0.000000f, -0.923880f, -0.382683f, // 121
    0.382683f, -0.923880f,  0.000000f, // 122
    0.673887f, -0.673887f,  0.302905f, // 123
    0.673887f, -0.673887f, -0.302905f, // 124
    0.382683f, -0.923880f,  0.000000f, // 125
    0.000000f, -0.923880f,  0.382683f, // 126
   -0.302905f, -0.673887f,  0.673887f, // 127
    0.302905f, -0.673887f,  0.673887f, // 128
    0.000000f, -0.923880f,  0.382683f, // 129
   -0.382683f, -0.923880f,  0.000000f, // 130
   -0.673887f, -0.673887f, -0.302905f, // 131
   -0.673887f, -0.673887f,  0.302905f, // 132
   -0.382683f, -0.923880f,  0.000000f, // 133
    0.000000f, -0.382683f,  0.923880f, // 134
    0.302905f, -0.673887f,  0.673887f, // 135
   -0.302905f, -0.673887f,  0.673887f, // 136
    0.000000f, -0.382683f,  0.923880f, // 137
    0.382683f,  0.000000f,  0.923880f, // 138
    0.673887f,  0.302905f,  0.673887f, // 139
    0.673887f, -0.302905f,  0.673887f, // 140
    0.382683f,  0.000000f,  0.923880f, // 141
    0.000000f,  0.382683f,  0.923880f, // 142
   -0.302905f,  0.673887f,  0.673887f, // 143
    0.302905f,  0.673887f,  0.673887f, // 144
    0.000000f,  0.382683f,  0.923880f, // 145
   -0.382683f,  0.000000f,  0.923880f, // 146
   -0.673887f, -0.302905f,  0.673887f, // 147
   -0.673887f,  0.302905f,  0.673887f, // 148
   -0.382683f,  0.000000f,  0.923880f, // 149
   -0.923880f,  0.382683f,  0.000000f, // 150
   -0.673887f,  0.673887f, -0.302905f, // 151
   -0.673887f,  0.673887f,  0.302905f, // 152
   -0.923880f,  0.382683f,  0.000000f, // 153
   -0.923880f,  0.000000f, -0.382683f, // 154
   -0.673887f, -0.302905f, -0.673887f, // 155
   -0.673887f,  0.302905f, -0.673887f, // 156
   -0.923880f,  0.000000f, -0.382683f, // 157
   -0.923880f, -0.382683f,  0.000000f, // 158
   -0.673887f, -0.673887f,  0.302905f, // 159
   -0.673887f, -0.673887f, -0.302905f, // 160
   -0.923880f, -0.382683f,  0.000000f, // 161
   -0.923880f,  0.000000f,  0.382683f, // 162
   -0.673887f,  0.302905f,  0.673887f, // 163
   -0.673887f, -0.302905f,  0.673887f, // 164
   -0.923880f,  0.000000f,  0.382683f, // 165
    0.382683f,  0.000000f, -0.923880f, // 166
    0.673887f, -0.302905f, -0.673887f, // 167
    0.673887f,  0.302905f, -0.673887f, // 168
    0.382683f,  0.000000f, -0.923880f, // 169
    0.000000f, -0.382683f, -0.923880f, // 170
   -0.302905f, -0.673887f, -0.673887f, // 171
    0.302905f, -0.673887f, -0.673887f, // 172
    0.000000f, -0.382683f, -0.923880f, // 173
   -0.382683f,  0.000000f, -0.923880f, // 174
   -0.673887f,  0.302905f, -0.673887f, // 175
   -0.673887f, -0.302905f, -0.673887f, // 176
   -0.382683f,  0.000000f, -0.923880f, // 177
    0.000000f,  0.382683f, -0.923880f, // 178
    0.302905f,  0.673887f, -0.673887f, // 179
   -0.302905f,  0.673887f, -0.673887f, // 180
    0.000000f,  0.382683f, -0.923880f, // 181
    0.535467f,  0.827549f, -0.168634f, // 182
    0.167277f,  0.971616f, -0.167277f, // 183
    0.464385f,  0.754117f, -0.464385f, // 184
    0.535467f,  0.827549f, -0.168634f, // 185
    0.535467f,  0.827549f,  0.168634f, // 186
    0.464385f,  0.754117f,  0.464385f, // 187
    0.167277f,  0.971616f,  0.167277f, // 188
    0.535467f,  0.827549f,  0.168634f, // 189
   -0.168634f,  0.827549f, -0.535467f, // 190
   -0.167277f,  0.971616f, -0.167277f, // 191
   -0.464385f,  0.754117f, -0.464385f, // 192
   -0.168634f,  0.827549f, -0.535467f, // 193
    0.168634f,  0.827549f, -0.535467f, // 194
    0.464385f,  0.754117f, -0.464385f, // 195
    0.167277f,  0.971616f, -0.167277f, // 196
    0.168634f,  0.827549f, -0.535467f, // 197
   -0.535467f,  0.827549f,  0.168634f, // 198
   -0.167277f,  0.971616f,  0.167277f, // 199
   -0.464385f,  0.754117f,  0.464385f, // 200
   -0.535467f,  0.827549f,  0.168634f, // 201
   -0.535467f,  0.827549f, -0.168634f, // 202
   -0.464385f,  0.754117f, -0.464385f, // 203
   -0.167277f,  0.971616f, -0.167277f, // 204
   -0.535467f,  0.827549f, -0.168634f, // 205
    0.168634f,  0.827549f,  0.535467f, // 206
    0.167277f,  0.971616f,  0.167277f, // 207
    0.464385f,  0.754117f,  0.464385f, // 208
    0.168634f,  0.827549f,  0.535467f, // 209
   -0.168634f,  0.827549f,  0.535467f, // 210
   -0.464385f,  0.754117f,  0.464385f, // 211
   -0.167277f,  0.971616f,  0.167277f, // 212
   -0.168634f,  0.827549f,  0.535467f, // 213
    0.827549f,  0.168634f, -0.535467f, // 214
    0.971616f,  0.167277f, -0.167277f, // 215
    0.754117f,  0.464385f, -0.464385f, // 216
    0.827549f,  0.168634f, -0.535467f, // 217
    0.827549f, -0.168634f, -0.535467f, // 218
    0.754117f, -0.464385f, -0.464385f, // 219
    0.971616f, -0.167277f, -0.167277f, // 220
    0.827549f, -0.168634f, -0.535467f, // 221
    0.827549f,  0.535467f,  0.168634f, // 222
    0.971616f,  0.167277f,  0.167277f, // 223
    0.754117f,  0.464385f,  0.464385f, // 224
    0.827549f,  0.535467f,  0.168634f, // 225
    0.827549f,  0.535467f, -0.168634f, // 226
    0.754117f,  0.464385f, -0.464385f, // 227
    0.971616f,  0.167277f, -0.167277f, // 228
    0.827549f,  0.535467f, -0.168634f, // 229
    0.827549f, -0.168634f,  0.535467f, // 230
    0.971616f, -0.167277f,  0.167277f, // 231
    0.754117f, -0.464385f,  0.464385f, // 232
    0.827549f, -0.168634f,  0.535467f, // 233
    0.827549f,  0.168634f,  0.535467f, // 234
    0.754117f,  0.464385f,  0.464385f, // 235
    0.971616f,  0.167277f,  0.167277f, // 236
    0.827549f,  0.168634f,  0.535467f, // 237
    0.827549f, -0.535467f, -0.168634f, // 238
    0.971616f, -0.167277f, -0.167277f, // 239
    0.754117f, -0.464385f, -0.464385f, // 240
    0.827549f, -0.535467f, -0.168634f, // 241
    0.827549f, -0.535467f,  0.168634f, // 242
    0.754117f, -0.464385f,  0.464385f, // 243
    0.971616f, -0.167277f,  0.167277f, // 244
    0.827549f, -0.535467f,  0.168634f, // 245
    0.168634f, -0.827549f, -0.535467f, // 246
    0.167277f, -0.971616f, -0.167277f, // 247
    0.464385f, -0.754117f, -0.464385f, // 248
    0.168634f, -0.827549f, -0.535467f, // 249
   -0.168634f, -0.827549f, -0.535467f, // 250
   -0.464385f, -0.754117f, -0.464385f, // 251
   -0.167277f, -0.971616f, -0.167277f, // 252
   -0.168634f, -0.827549f, -0.535467f, // 253
    0.535467f, -0.827549f,  0.168634f, // 254
    0.167277f, -0.971616f,  0.167277f, // 255
    0.464385f, -0.754117f,  0.464385f, // 256
    0.535467f, -0.827549f,  0.168634f, // 257
    0.535467f, -0.827549f, -0.168634f, // 258
    0.464385f, -0.754117f, -0.464385f, // 259
    0.167277f, -0.971616f, -0.167277f, // 260
    0.535467f, -0.827549f, -0.168634f, // 261
   -0.168634f, -0.827549f,  0.535467f, // 262
   -0.167277f, -0.971616f,  0.167277f, // 263
   -0.464385f, -0.754117f,  0.464385f, // 264
   -0.168634f, -0.827549f,  0.535467f, // 265
    0.168634f, -0.827549f,  0.535467f, // 266
    0.464385f, -0.754117f,  0.464385f, // 267
    0.167277f, -0.971616f,  0.167277f, // 268
    0.168634f, -0.827549f,  0.535467f, // 269
   -0.535467f, -0.827549f, -0.168634f, // 270
   -0.167277f, -0.971616f, -0.167277f, // 271
   -0.464385f, -0.754117f, -0.464385f, // 272
   -0.535467f, -0.827549f, -0.168634f, // 273
   -0.535467f, -0.827549f,  0.168634f, // 274
   -0.464385f, -0.754117f,  0.464385f, // 275
   -0.167277f, -0.971616f,  0.167277f, // 276
   -0.535467f, -0.827549f,  0.168634f, // 277
    0.168634f, -0.535467f,  0.827549f, // 278
    0.167277f, -0.167277f,  0.971616f, // 279
    0.464385f, -0.464385f,  0.754117f, // 280
    0.168634f, -0.535467f,  0.827549f, // 281
   -0.168634f, -0.535467f,  0.827549f, // 282
   -0.464385f, -0.464385f,  0.754117f, // 283
   -0.167277f, -0.167277f,  0.971616f, // 284
   -0.168634f, -0.535467f,  0.827549f, // 285
    0.535467f,  0.168634f,  0.827549f, // 286
    0.167277f,  0.167277f,  0.971616f, // 287
    0.464385f,  0.464385f,  0.754117f, // 288
    0.535467f,  0.168634f,  0.827549f, // 289
    0.535467f, -0.168634f,  0.827549f, // 290
    0.464385f, -0.464385f,  0.754117f, // 291
    0.167277f, -0.167277f,  0.971616f, // 292
    0.535467f, -0.168634f,  0.827549f, // 293
   -0.168634f,  0.535467f,  0.827549f, // 294
   -0.167277f,  0.167277f,  0.971616f, // 295
   -0.464385f,  0.464385f,  0.754117f, // 296
   -0.168634f,  0.535467f,  0.827549f, // 297
    0.168634f,  0.535467f,  0.827549f, // 298
    0.464385f,  0.464385f,  0.754117f, // 299
    0.167277f,  0.167277f,  0.971616f, // 300
    0.168634f,  0.535467f,  0.827549f, // 301
   -0.535467f, -0.168634f,  0.827549f, // 302
   -0.167277f, -0.167277f,  0.971616f, // 303
   -0.464385f, -0.464385f,  0.754117f, // 304
   -0.535467f, -0.168634f,  0.827549f, // 305
   -0.535467f,  0.168634f,  0.827549f, // 306
   -0.464385f,  0.464385f,  0.754117f, // 307
   -0.167277f,  0.167277f,  0.971616f, // 308
   -0.535467f,  0.168634f,  0.827549f, // 309
   -0.827549f,  0.535467f, -0.168634f, // 310
   -0.971616f,  0.167277f, -0.167277f, // 311
   -0.754117f,  0.464385f, -0.464385f, // 312
   -0.827549f,  0.535467f, -0.168634f, // 313
   -0.827549f,  0.535467f,  0.168634f, // 314
   -0.754117f,  0.464385f,  0.464385f, // 315
   -0.971616f,  0.167277f,  0.167277f, // 316
   -0.827549f,  0.535467f,  0.168634f, // 317
   -0.827549f, -0.168634f, -0.535467f, // 318
   -0.971616f, -0.167277f, -0.167277f, // 319
   -0.754117f, -0.464385f, -0.464385f, // 320
   -0.827549f, -0.168634f, -0.535467f, // 321
   -0.827549f,  0.168634f, -0.535467f, // 322
   -0.754117f,  0.464385f, -0.464385f, // 323
   -0.971616f,  0.167277f, -0.167277f, // 324
   -0.827549f,  0.168634f, -0.535467f, // 325
   -0.827549f, -0.535467f,  0.168634f, // 326
   -0.971616f, -0.167277f,  0.167277f, // 327
   -0.754117f, -0.464385f,  0.464385f, // 328
   -0.827549f, -0.535467f,  0.168634f, // 329
   -0.827549f, -0.535467f, -0.168634f, // 330
   -0.754117f, -0.464385f, -0.464385f, // 331
   -0.971616f, -0.167277f, -0.167277f, // 332
   -0.827549f, -0.535467f, -0.168634f, // 333
   -0.827549f,  0.168634f,  0.535467f, // 334
   -0.971616f,  0.167277f,  0.167277f, // 335
   -0.754117f,  0.464385f,  0.464385f, // 336
   -0.827549f,  0.168634f,  0.535467f, // 337
   -0.827549f, -0.168634f,  0.535467f, // 338
   -0.754117f, -0.464385f,  0.464385f, // 339
   -0.971616f, -0.167277f,  0.167277f, // 340
   -0.827549f, -0.168634f,  0.535467f, // 341
    0.535467f, -0.168634f, -0.827549f, // 342
    0.167277f, -0.167277f, -0.971616f, // 343
    0.464385f, -0.464385f, -0.754117f, // 344
    0.535467f, -0.168634f, -0.827549f, // 345
    0.535467f,  0.168634f, -0.827549f, // 346
    0.464385f,  0.464385f, -0.754117f, // 347
    0.167277f,  0.167277f, -0.971616f, // 348
    0.535467f,  0.168634f, -0.827549f, // 349
   -0.168634f, -0.535467f, -0.827549f, // 350
   -0.167277f, -0.167277f, -0.971616f, // 351
   -0.464385f, -0.464385f, -0.754117f, // 352
   -0.168634f, -0.535467f, -0.827549f, // 353
    0.168634f, -0.535467f, -0.827549f, // 354
    0.464385f, -0.464385f, -0.754117f, // 355
    0.167277f, -0.167277f, -0.971616f, // 356
    0.168634f, -0.535467f, -0.827549f, // 357
   -0.535467f,  0.168634f, -0.827549f, // 358
   -0.167277f,  0.167277f, -0.971616f, // 359
   -0.464385f,  0.464385f, -0.754117f, // 360
   -0.535467f,  0.168634f, -0.827549f, // 361
   -0.535467f, -0.168634f, -0.827549f, // 362
   -0.464385f, -0.464385f, -0.754117f, // 363
   -0.167277f, -0.167277f, -0.971616f, // 364
   -0.535467f, -0.168634f, -0.827549f, // 365
    0.168634f,  0.535467f, -0.827549f, // 366
    0.167277f,  0.167277f, -0.971616f, // 367
    0.464385f,  0.464385f, -0.754117f, // 368
    0.168634f,  0.535467f, -0.827549f, // 369
   -0.168634f,  0.535467f, -0.827549f, // 370
   -0.464385f,  0.464385f, -0.754117f, // 371
   -0.167277f,  0.167277f, -0.971616f, // 372
   -0.168634f,  0.535467f, -0.827549f  // 373
  };
const unsigned short idx4[1152]  = {// [1152]
    38, 86, 182,// 0
    86, 14, 182,// 1
    8, 86, 183,// 2
    86, 38, 183,// 3
    38, 87, 184,// 4
    87, 5, 184,// 5
    14, 87, 185,// 6
    87, 38, 185,// 7
    39, 88, 186,// 8
    88, 14, 186,// 9
    1, 88, 187,// 10
    88, 39, 187,// 11
    39, 89, 188,// 12
    89, 8, 188,// 13
    14, 89, 189,// 14
    89, 39, 189,// 15
    40, 90, 190,// 16
    90, 15, 190,// 17
    8, 90, 191,// 18
    90, 40, 191,// 19
    40, 91, 192,// 20
    91, 4, 192,// 21
    15, 91, 193,// 22
    91, 40, 193,// 23
    41, 92, 194,// 24
    92, 15, 194,// 25
    5, 92, 195,// 26
    92, 41, 195,// 27
    41, 93, 196,// 28
    93, 8, 196,// 29
    15, 93, 197,// 30
    93, 41, 197,// 31
    42, 94, 198,// 32
    94, 16, 198,// 33
    8, 94, 199,// 34
    94, 42, 199,// 35
    42, 95, 200,// 36
    95, 0, 200,// 37
    16, 95, 201,// 38
    95, 42, 201,// 39
    43, 96, 202,// 40
    96, 16, 202,// 41
    4, 96, 203,// 42
    96, 43, 203,// 43
    43, 97, 204,// 44
    97, 8, 204,// 45
    16, 97, 205,// 46
    97, 43, 205,// 47
    44, 98, 206,// 48
    98, 17, 206,// 49
    8, 98, 207,// 50
    98, 44, 207,// 51
    44, 99, 208,// 52
    99, 1, 208,// 53
    17, 99, 209,// 54
    99, 44, 209,// 55
    45, 100, 210,// 56
    100, 17, 210,// 57
    0, 100, 211,// 58
    100, 45, 211,// 59
    45, 101, 212,// 60
    101, 8, 212,// 61
    17, 101, 213,// 62
    101, 45, 213,// 63
    46, 102, 214,// 64
    102, 18, 214,// 65
    9, 102, 215,// 66
    102, 46, 215,// 67
    46, 103, 216,// 68
    103, 5, 216,// 69
    18, 103, 217,// 70
    103, 46, 217,// 71
    47, 104, 218,// 72
    104, 18, 218,// 73
    7, 104, 219,// 74
    104, 47, 219,// 75
    47, 105, 220,// 76
    105, 9, 220,// 77
    18, 105, 221,// 78
    105, 47, 221,// 79
    48, 106, 222,// 80
    106, 19, 222,// 81
    9, 106, 223,// 82
    106, 48, 223,// 83
    48, 107, 224,// 84
    107, 1, 224,// 85
    19, 107, 225,// 86
    107, 48, 225,// 87
    49, 108, 226,// 88
    108, 19, 226,// 89
    5, 108, 227,// 90
    108, 49, 227,// 91
    49, 109, 228,// 92
    109, 9, 228,// 93
    19, 109, 229,// 94
    109, 49, 229,// 95
    50, 110, 230,// 96
    110, 20, 230,// 97
    9, 110, 231,// 98
    110, 50, 231,// 99
    50, 111, 232,// 100
    111, 3, 232,// 101
    20, 111, 233,// 102
    111, 50, 233,// 103
    51, 112, 234,// 104
    112, 20, 234,// 105
    1, 112, 235,// 106
    112, 51, 235,// 107
    51, 113, 236,// 108
    113, 9, 236,// 109
    20, 113, 237,// 110
    113, 51, 237,// 111
    52, 114, 238,// 112
    114, 21, 238,// 113
    9, 114, 239,// 114
    114, 52, 239,// 115
    52, 115, 240,// 116
    115, 7, 240,// 117
    21, 115, 241,// 118
    115, 52, 241,// 119
    53, 116, 242,// 120
    116, 21, 242,// 121
    3, 116, 243,// 122
    116, 53, 243,// 123
    53, 117, 244,// 124
    117, 9, 244,// 125
    21, 117, 245,// 126
    117, 53, 245,// 127
    54, 118, 246,// 128
    118, 22, 246,// 129
    10, 118, 247,// 130
    118, 54, 247,// 131
    54, 119, 248,// 132
    119, 7, 248,// 133
    22, 119, 249,// 134
    119, 54, 249,// 135
    55, 120, 250,// 136
    120, 22, 250,// 137
    6, 120, 251,// 138
    120, 55, 251,// 139
    55, 121, 252,// 140
    121, 10, 252,// 141
    22, 121, 253,// 142
    121, 55, 253,// 143
    56, 122, 254,// 144
    122, 23, 254,// 145
    10, 122, 255,// 146
    122, 56, 255,// 147
    56, 123, 256,// 148
    123, 3, 256,// 149
    23, 123, 257,// 150
    123, 56, 257,// 151
    57, 124, 258,// 152
    124, 23, 258,// 153
    7, 124, 259,// 154
    124, 57, 259,// 155
    57, 125, 260,// 156
    125, 10, 260,// 157
    23, 125, 261,// 158
    125, 57, 261,// 159
    58, 126, 262,// 160
    126, 24, 262,// 161
    10, 126, 263,// 162
    126, 58, 263,// 163
    58, 127, 264,// 164
    127, 2, 264,// 165
    24, 127, 265,// 166
    127, 58, 265,// 167
    59, 128, 266,// 168
    128, 24, 266,// 169
    3, 128, 267,// 170
    128, 59, 267,// 171
    59, 129, 268,// 172
    129, 10, 268,// 173
    24, 129, 269,// 174
    129, 59, 269,// 175
    60, 130, 270,// 176
    130, 25, 270,// 177
    10, 130, 271,// 178
    130, 60, 271,// 179
    60, 131, 272,// 180
    131, 6, 272,// 181
    25, 131, 273,// 182
    131, 60, 273,// 183
    61, 132, 274,// 184
    132, 25, 274,// 185
    2, 132, 275,// 186
    132, 61, 275,// 187
    61, 133, 276,// 188
    133, 10, 276,// 189
    25, 133, 277,// 190
    133, 61, 277,// 191
    62, 134, 278,// 192
    134, 26, 278,// 193
    11, 134, 279,// 194
    134, 62, 279,// 195
    62, 135, 280,// 196
    135, 3, 280,// 197
    26, 135, 281,// 198
    135, 62, 281,// 199
    63, 136, 282,// 200
    136, 26, 282,// 201
    2, 136, 283,// 202
    136, 63, 283,// 203
    63, 137, 284,// 204
    137, 11, 284,// 205
    26, 137, 285,// 206
    137, 63, 285,// 207
    64, 138, 286,// 208
    138, 27, 286,// 209
    11, 138, 287,// 210
    138, 64, 287,// 211
    64, 139, 288,// 212
    139, 1, 288,// 213
    27, 139, 289,// 214
    139, 64, 289,// 215
    65, 140, 290,// 216
    140, 27, 290,// 217
    3, 140, 291,// 218
    140, 65, 291,// 219
    65, 141, 292,// 220
    141, 11, 292,// 221
    27, 141, 293,// 222
    141, 65, 293,// 223
    66, 142, 294,// 224
    142, 28, 294,// 225
    11, 142, 295,// 226
    142, 66, 295,// 227
    66, 143, 296,// 228
    143, 0, 296,// 229
    28, 143, 297,// 230
    143, 66, 297,// 231
    67, 144, 298,// 232
    144, 28, 298,// 233
    1, 144, 299,// 234
    144, 67, 299,// 235
    67, 145, 300,// 236
    145, 11, 300,// 237
    28, 145, 301,// 238
    145, 67, 301,// 239
    68, 146, 302,// 240
    146, 29, 302,// 241
    11, 146, 303,// 242
    146, 68, 303,// 243
    68, 147, 304,// 244
    147, 2, 304,// 245
    29, 147, 305,// 246
    147, 68, 305,// 247
    69, 148, 306,// 248
    148, 29, 306,// 249
    0, 148, 307,// 250
    148, 69, 307,// 251
    69, 149, 308,// 252
    149, 11, 308,// 253
    29, 149, 309,// 254
    149, 69, 309,// 255
    70, 150, 310,// 256
    150, 30, 310,// 257
    12, 150, 311,// 258
    150, 70, 311,// 259
    70, 151, 312,// 260
    151, 4, 312,// 261
    30, 151, 313,// 262
    151, 70, 313,// 263
    71, 152, 314,// 264
    152, 30, 314,// 265
    0, 152, 315,// 266
    152, 71, 315,// 267
    71, 153, 316,// 268
    153, 12, 316,// 269
    30, 153, 317,// 270
    153, 71, 317,// 271
    72, 154, 318,// 272
    154, 31, 318,// 273
    12, 154, 319,// 274
    154, 72, 319,// 275
    72, 155, 320,// 276
    155, 6, 320,// 277
    31, 155, 321,// 278
    155, 72, 321,// 279
    73, 156, 322,// 280
    156, 31, 322,// 281
    4, 156, 323,// 282
    156, 73, 323,// 283
    73, 157, 324,// 284
    157, 12, 324,// 285
    31, 157, 325,// 286
    157, 73, 325,// 287
    74, 158, 326,// 288
    158, 32, 326,// 289
    12, 158, 327,// 290
    158, 74, 327,// 291
    74, 159, 328,// 292
    159, 2, 328,// 293
    32, 159, 329,// 294
    159, 74, 329,// 295
    75, 160, 330,// 296
    160, 32, 330,// 297
    6, 160, 331,// 298
    160, 75, 331,// 299
    75, 161, 332,// 300
    161, 12, 332,// 301
    32, 161, 333,// 302
    161, 75, 333,// 303
    76, 162, 334,// 304
    162, 33, 334,// 305
    12, 162, 335,// 306
    162, 76, 335,// 307
    76, 163, 336,// 308
    163, 0, 336,// 309
    33, 163, 337,// 310
    163, 76, 337,// 311
    77, 164, 338,// 312
    164, 33, 338,// 313
    2, 164, 339,// 314
    164, 77, 339,// 315
    77, 165, 340,// 316
    165, 12, 340,// 317
    33, 165, 341,// 318
    165, 77, 341,// 319
    78, 166, 342,// 320
    166, 34, 342,// 321
    13, 166, 343,// 322
    166, 78, 343,// 323
    78, 167, 344,// 324
    167, 7, 344,// 325
    34, 167, 345,// 326
    167, 78, 345,// 327
    79, 168, 346,// 328
    168, 34, 346,// 329
    5, 168, 347,// 330
    168, 79, 347,// 331
    79, 169, 348,// 332
    169, 13, 348,// 333
    34, 169, 349,// 334
    169, 79, 349,// 335
    80, 170, 350,// 336
    170, 35, 350,// 337
    13, 170, 351,// 338
    170, 80, 351,// 339
    80, 171, 352,// 340
    171, 6, 352,// 341
    35, 171, 353,// 342
    171, 80, 353,// 343
    81, 172, 354,// 344
    172, 35, 354,// 345
    7, 172, 355,// 346
    172, 81, 355,// 347
    81, 173, 356,// 348
    173, 13, 356,// 349
    35, 173, 357,// 350
    173, 81, 357,// 351
    82, 174, 358,// 352
    174, 36, 358,// 353
    13, 174, 359,// 354
    174, 82, 359,// 355
    82, 175, 360,// 356
    175, 4, 360,// 357
    36, 175, 361,// 358
    175, 82, 361,// 359
    83, 176, 362,// 360
    176, 36, 362,// 361
    6, 176, 363,// 362
    176, 83, 363,// 363
    83, 177, 364,// 364
    177, 13, 364,// 365
    36, 177, 365,// 366
    177, 83, 365,// 367
    84, 178, 366,// 368
    178, 37, 366,// 369
    13, 178, 367,// 370
    178, 84, 367,// 371
    84, 179, 368,// 372
    179, 5, 368,// 373
    37, 179, 369,// 374
    179, 84, 369,// 375
    85, 180, 370,// 376
    180, 37, 370,// 377
    4, 180, 371,// 378
    180, 85, 371,// 379
    85, 181, 372,// 380
    181, 13, 372,// 381
    37, 181, 373,// 382
    181, 85, 373 // 383
  };

typedef QList<MyBond> Connection;
typedef QList<MyAtom> CEnvironment;
//! Unit cell parameters
struct Cell {
  //! the dimension a in Angstrom
  double a;
  //! the dimension b in Angstrom
  double b;
  //! the dimension c in Angstrom
  double c;
  //! the angle alpha in degrees
  double al;
  //! the angle beta in degrees
  double be;
  //! the angle gamma in degrees
  double ga;
  //! \f$\varphi =  \sqrt(1 - (\cos(\alpha)^2) - (\cos(\beta)^2) - (\cos(\gamma)^2) + 2\cos(\alpha)\cos(\beta)\cos(\gamma))\f$
  double phi;
  //! the cell volume in Angstrom^3
  double V;
  //! the reciprocal dimension a 
  double as;
  //! the reciprocal dimension b 
  double bs;
  //! the reciprocal dimension c 
  double cs;
  //! \f$ \tau = c ((\cos(\alpha) - \cos(\beta)  \cos(\gamma)) / \sin(\gamma))\f$
  double tau;
  //! \f$ \cos(\gamma)\f$ 
  double cosga;
  //! \f$ \cos(\alpha) \f$ 
  double cosal;
  //! \f$ \cos(\beta) \f$ 
  double cosbe;
  //! \f$ \sin(\gamma) \f$ 
  double singa;
  //! \f$ \tan(\gamma) \f$ 
  double tanga;
  //! List of symmetry operators
  QList<Matrix> symmops;
  //! List of translations
  QList<V3> trans;
  //! space group is centred (A,B,C,I,F)
  bool centered;
   //! space group is centro symmetric
  bool centeric;
  //! ZERR string as it is from the res file
  QString Z;
  //! the wavelenth  \f$ \lambda  \f$ in Angstroms
  double wave;
};
//! shortest distance matrix item type
struct SdmItem{
  //! shortest contact distance
  double d;
  //! atom index of the starting atom
  int a1;
  //! atom index of the ending atom
  int a2;
  //! symmetry number of the contact
  int sn;
  V3 floorD;
  //! contact is covalent
  bool covalent;
};

inline bool operator < (const SdmItem &a1, const SdmItem &a2){
  double d1=a1.d;//a1.a2*99999+a1.d;
  double d2=a2.d;//a2.a2*99999+a2.d;
  return (d1<d2);
}
//!Knopf is a list of neighbors of an atom
struct Knopf{
  //! neighbors is list of atom indices of neigbours of that atom
QList<int> neighbors;
};
/*! \brief Molecule is a crystallography and molecular structure object of shelXle. 
 *
 * It has routines to convert coordinates and 
 * ADPs from fractional to cartesian space, symmetry operations etc. It provides also routintes to draw atoms bonds and q-peaks. 
 * It is thought that there is only one instance of this class in ShelXle during runtime. Molecule accesses the settings file of ShelXle.
 */
class Molecule{
public:
	bool nopm1;//!< No part minus n ghost
    bool highlightEquivalents;//!< Symmetry generated objects will be drawn in lighter colors if this is true
	bool qbeforehkl;//!< There are Q-Peaks before HKLF instruction 
        bool growQPeak;//!< Q-Peaks should be treated with symmetry operators
	bool monoQrom;//!< Q-Peaks in a single color instead of a gradient.
       QColor mQolor;//!< the single Q-Peak color
       double  qboMin,qboMax;
  int LOD;//!< The Level of Detail
  Molecule();//!<The constructor
  void setHBondMaxDist(double d);
  void setHBondMaxAngl(double w);
  double hbdist();
  double hbangl();
  void Farbverlauf (GLfloat wrt,GLfloat min,GLfloat max);//!<computes a gradient color for wrt which is between min and max and calls glColor. @param wrt value for which a color should be assigned. @param min minimum value of wrt.  @param max maximum value of wrt.
  Cell cell;//!<The unit cell parameters
  QString Fragments;//!<A html string of the kind "<b>The asymmetric unit contains N fragments.</b><br>" were N is the number of fragments.
  QList<SdmItem> sdm;//!< shortest distance matrix.
  QList<SdmItem> envi_sdm;//!< shortest distance matrix for the ENVIron ment listing functionality.
  QList<SdmItem> contact;//!< shortest distance matrix with possible hydrogen bonds for auto HFIX.
  QList<Matrix> symmopsEQIV;//!< list of symmetry operations from  EQIV instructions.
  QStringList labelEQIV;//!< list of atom lable from  EQIV instructions.
  QList<V3> transEQIV;//!< list of translation from  EQIV instructions.
  QMap<QString,int> eqivMap;//!< maps labelEQIV and symmetry operation indices.
  QList<MyBind> freeatoms,//!< bond list for FREE instructions
	  bindatoms;//!< bond list for BIND instructions 
  QList<Knopf> knoepfe;//!< neighbors list of each atom
  int adp,//!<if non zero the atoms and bons are drwn in ellipsoid style.
      intern,
      bondColorStyle;//!< uni colored bonds or bonds half and half colored like the connected atoms.
  double bondStrength//!< cylinder radius of the bonds in Angstrom
	  ,qPeakRad;//!< icosahedron size in Angstrom
  GLuint hbtex; //!< texture handle for hydrogen bonds
//	 hbtex2; //!< texture handle for
  GLuint adpwall,//!< texture handle elipsoid walls
	 adpwall_plaid;//!< texture handle elipsoid walls different style
 // int nonPositiveDefinite;
  int proba,//!< probability level of the ellipsoids
      tubes,//!< tube mode instead of ball stick 
      dratom;//!< wire style of atoms
  //V3 VZ;
  QColor AtomColor[109];//!<List of colors for all supported elements.
  int AtomStyle[109];//!<List of atom style types for all supported elements.
  double arad[109];//!<List of ball radii for ball stick visulaization  for all supported elements.
  unsigned short Kovalenz_Radien[109];//!<List of covalent radii for all supported elements.
  int pseSize;//!< Font size for the PSEWidget to be stored in the settings.
  double  pmin,//!< minimum peak height of the Q-Peaks.
	  pmax;//!< maximum peak height of the Q-Peaks.
  QColor bondColor;//!< the color of the bonds in uni colored mode.

  QColor enviBondColor,//!< covalent color in ENVI
      enviHBColor,//!< hydrogen bond color in ENVI
      enviDefaultColor;//!< default color in ENVI
  CEnvironment asymm,//!< atom list containing the asymertic unit 
	       showatoms,//!< atom list containing all atoms to be drawn.
	       selectedatoms,//!< atom list of selected atoms.
	       duplicateAtoms;//!< atom list of identcal labeled atoms.
  Connection showbonds;//!< list of visible bonds.
  Connection lbonds;//!< list of line bonds (for Q-Peaks)
  QStringList usedSymmetry;//!< List of internal symmetry codes of symmetry operations curently applied.
  QString HumanSymmetry;//!< A human readable list of symmetry operations in use.
  QSettings *einstellung;//!< the QSetttings for ShelXle.
  //unsigned short ElNeg[83];
  //
  //
  QStringList sdmcompleter();
  void atoms(CEnvironment atom,int proba=50);
  void bonds(Connection bond);
  void dbond(Connection bond);
  void lbond();
  QString h_bonds(Connection bond,CEnvironment atoms);
  void h_bonds2(Connection bond,CEnvironment atoms);
  void unitCell();
  Connection connecting(const CEnvironment &atom, bool eac=false);
  bool decodeSymmCard(const QString symmCard);
  bool decodeSymmCardEQIV(const QString symmCard);
  //void countMols(QList<INP> & xdinp);
  //bool applyLatticeCentro(const QChar latt,const bool centro);
  QString symmcode2human(QStringList brauchSymm);//!< converts a list of internal symmetry codes in human readable ones.
  QString symmcode2human(QString brauchSymm);//!< converts an internal symmetry code into human readable symmetry operation description
  QString symmcode2human(QString brauchSymm, int j);//!< converts an internal symmetry code into human readable symmetry operation description prepends french quotes j: 
  QString symmcode2human(int s);
  QString symmCard2Code(QString symmCard);
  void frac2kart (V3 x, V3 & y);
  void kart2frac (V3 x, V3 & y);
  int getOZ(QString S1);//!< returns the atomic number (starting from 0) for a given chmical elememt symbol S1. @param S1 element symbol.
  double shortestDistance(QString sc);
  static double winkel(V3 a,V3 b);//!< calculates the angle in degrees between two vectors 
  double dieder(V3 a,V3 b, V3 c);//!< calculates a torsion angle from 3 given vectors
  static V3 kreuzX(double x1,double y1,double z1,double x2,double y2,double z2) ;//!< a cross product 
  double fl(double x,double y, double z);//!< calculates the length of a vector given by x,y,z in fractional coordinates in Angstroms @param x,y,z fractional coordinates of a vector. 
  double wl(V3 f2, V3 f1, V3 f3);//!< calculates angle in degrees of the 3 positional vectors in fractional space. 
  void cellSetup();//!< calculates unit cell parameters from a,b,c, alpha, beta, gammma
  void fuse();
  void grow();
  void fillCell();
  void uniqueInCell();
  void packer(QStringList brauchSymm);
  void applyLatticeCentro(int gitter);
  void Uf2Uo(const Matrix x, Matrix & y) ;
  void Usym (Matrix x,Matrix sym, Matrix & y);
  double dimension();
  void expandAt(int index);
  void expandAll();
  void complete();
  void enviSDM(double range);
  QString pse(int oz);//!< returns the chemical symbol for the given atomic number (starting from 0)
  void loadSettings();
  void ikosa(double R);//!< draws an icosahedron with radius R. @param R radius in Anstrom.
  int maxmols(){return maxmol;}//!< returns the number of fragments (molecules)
private:
  double HAMax,HAWink;
  void ellipse(int style);
  void hirsh(Connection bond);
  void sphere(int adp);
  void dCube(GLfloat rad);
//  void ikosa(double R);
//  V3 eigenvalues(const Matrix m);
//  Matrix eigenvectors(const Matrix m,const V3 l);
  double * jacobi(const Matrix &uij, V3 &ev);
int maxmol;

};
#endif
