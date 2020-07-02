/// \file Matrix44.hpp
#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <cmath>
#include <algorithm>

#include "Vector3.hpp"

#define m11 matrix[0] ///< Position 0 en X et 0 en Y.
#define m12 matrix[1] ///< Position 1 en X et 0 en Y.
#define m13 matrix[2] ///< Position 2 en X et 0 en Y.
#define m14 matrix[3] ///< Position 3 en X et 0 en Y.
#define m21 matrix[4] ///< Position 0 en X et 1 en Y.
#define m22 matrix[5] ///< Position 1 en X et 1 en Y.
#define m23 matrix[6] ///< Position 2 en X et 1 en Y.
#define m24 matrix[7] ///< Position 3 en X et 1 en Y.
#define m31 matrix[8] ///< Position 0 en X et 2 en Y.
#define m32 matrix[9] ///< Position 1 en X et 2 en Y.
#define m33 matrix[10] ///< Position 2 en X et 2 en Y.
#define m34 matrix[11] ///< Position 3 en X et 2 en Y.
#define m41 matrix[12] ///< Position 0 en X et 3 en Y.
#define m42 matrix[13] ///< Position 1 en X et 3 en Y.
#define m43 matrix[14] ///< Position 2 en X et 3 en Y.
#define m44 matrix[15] ///< Position 3 en X et 3 en Y.

/// \brief Représentation d'un matrice 4x4.
template <typename T>
class Matrix44 {
public:
  T matrix[16]; ///< Matrice 4x4

  Matrix44() {
    std::fill(&m11, &m44, 0.0);
    m11 = m22 = m33 = m44 = 1.0;
  }

	/// \brief Transposition de la matrice.
  void Transpose() {
    swap(m12, m21);
    swap(m13, m31);
    swap(m14, m41);
    swap(m23, m32);
    swap(m24, m42);
    swap(m34, m43);
  }

  /// \brief Chargement de la matrice orthogonale.
  /// \param height Hauteur orthogonale.
  /// \param width Largeur orthogonale.
  void LoadOrthogonal(const double& width, const double& height){
	  m11 = 2.0 / width;
    m22 = 2.0 / -height;
    m33 = -1.0;
    m41 = -1.0;
    m42 = 1.0;
  }

  /// \brief Chargement de la matrice de vue.
  /// \param front vecteur pointant le devant.
  /// \param right Vecteur pointant à droite.
  /// \param up Vecteur pointant en haut.
  void LoadView(const Vector3& front, const Vector3& right, const Vector3& up){
    m11 = right.x; m12 = up.x; m13 = -front.x;
    m21 = right.y; m22 = up.y; m23 = -front.y;
    m31 = right.z; m32 = up.z; m33 = -front.z;
	}
	
	/// \brief Chargement de la matrice de projection.
	/// \param right Distance de la droite.
	/// \param top Distance du dessus.
	/// \param nearPlane Distance de l'avant plan.
	/// \param farPlane Distance de l'arrière plan.
  void LoadPerspective(const double& right, const double& top, const double& nearPlane, const double& farPlane) {
    m11 = nearPlane / right;
    m22 = nearPlane / top;
    m33 = (-(farPlane + nearPlane)) / (farPlane - nearPlane);
    m34 = -1.0;
    m43 = (-2.0 * farPlane * nearPlane) / (farPlane - nearPlane);
    m44 = 0.0;
	}
	
	/// \brief Chargement de la matrice de rotation sur l'axe des x.
	/// \param angle Angle en Radian.
	void LoadXRotation(const double& angle) {
		const double c = cos(angle);
		const double s = sin(angle);

		m22 = c; m23 = -s;
		m32 = s; m33 =  c;
	}
	
	/// \brief Chargement de la matrice de rotation sur l'axe des y.
	/// \param angle Angle en Radian.
	void LoadYRotation(const double& angle) {
		const double c = cos(angle);
		const double s = sin(angle);

		m11 =  c; m13 = s;
		m31 = -s; m33 = c;
	}

	/// \brief Charge de la matrice de rotation sur l'axe des z.
	/// \param angle Angle en Radian.
	void LoadZRotation(const double& angle) {
		const double c = cos(angle);
		const double s = sin(angle);

		m11 = c; m12 = -s;
		m21 = s; m22 =  c;
	}
	
	/// \brief Charge une matrice de rotation sur un axe arbitraire.
	/// \param angle Angle en Radian.
	/// \param axis Axe de rotation normalisée.
	void LoadRotation(const double& angle, const Vector3& axis) {
		const double c = cos(angle), omc = -c;
		const double s = sin(angle);

    	m11 = axis.x * axis.x * omc + c;          m12 = axis.x * axis.y * omc - axis.z * s; m13 = axis.x * axis.z * omc + axis.y * s;
    	m21 = axis.x * axis.y * omc + axis.z * s; m22 = axis.y * axis.y * omc + c;          m23 = axis.y * axis.z * omc - axis.x * s;
		m31 = axis.x * axis.z * omc - axis.y * s; m32 = axis.y * axis.z * omc + axis.x * s; m33 = axis.z * axis.z * omc + c;
	}

    /// \brief Applique une matrice de rotation sur un vecteur.
    /// \param v Vecteur sur lequel appliquer la rotation.
    void ApplyRotation(Vector3& v){
        v.x = m11 * v.x + m12 * v.y + m13 * v.z;
        v.y = m21 * v.x + m22 * v.y + m23 * v.z;
        v.z = m31 * v.x + m32 * v.y + m33 * v.z;
        v /= v.GetNorm();
    }

	/// \brief Charge une matrice de mise à l'échelle.
	/// \param scalar Nombre réel.
	inline void LoadScale(const double& scalar) {
		m11 = m22 = m33 = scalar;
	}

    /// \brief Charge une matrice d'étirement.
    /// \param v Vecteur d'étirement
    inline void LoadStretch(Vector3 v) {
        m11 = v.x;
        m22 = v.y;
        m33 = v.z;
    }

    /// \brief Charge une matrice d'étirement X.
    /// \param scalar Nombre réel.
    inline void LoadXStretch(const double& scalar) {
        m11 = scalar;
    }

    /// \brief Charge une matrice d'étirement Y.
    /// \param scalar Nombre réel.
    inline void LoadYStretch(const double& scalar) {
        m22 = scalar;
    }

    /// \brief Charge une matrice d'étirement Z.
    /// \param scalar Nombre réel.
    inline void LoadZStretch(const double& scalar) {
        m33 = scalar;
    }

	/// \brief Règle les coordonnées.
	/// \param vector coordonnée 3D.
	void SetPosition(Vector3 vector) {
		m41 = vector.x;
		m42 = vector.y;
		m43 = vector.z;
	}
};

typedef Matrix44<int> Matrix44i;
typedef Matrix44<float> Matrix44f;
typedef Matrix44<double> Matrix44d;

#endif
