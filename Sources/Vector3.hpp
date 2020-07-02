/// \file Vector3.hpp
#ifndef VECTOR3_H
#define VECTOR3_H

#include "Vector2.hpp"

struct Vector3 {
	double x; ///< Composante sur l'axe des 'x'.
	double y; ///< Composante sur l'axe des 'y'.
	double z; ///< Composante sur l'axe des 'z'.
	
	Vector3() {
		x = y = z = 0.0;
	}
	
	/// \param x Coordonnée en X.
	/// \param y Coordonnée en Y.
	/// \param z Coordonnée en Z.
	Vector3(double x, double y, double z){
		this->x = x;
		this->y = y;
		this->z = z;
	}
	
	/// \brief Calcule la norme du vecteur.
	/// \return Le type de retour est un double.
	inline double GetNorm() {
		return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	}
	
	/// \brief Calcule l'angle entre 2 vecteurs.
	/// \param au "au" indique si l'angle est calculé en degré ou en radian.
	/// \param v "v" est un vecteur en3 dimensions.s.
	/// \return Le type de retour est un double.
	double GetAngle(Vector3 v, AngleUnit au) {
		double norm = GetNorm();
		if(au) 
            return (acos((x * v.x) + (y * v.y) + (z * v.z)) / (norm * v.GetNorm()) * 180) / 3.14159265359;
        return (acos((x * v.x) + (y * v.y) + (z * v.z)) / (norm * v.GetNorm()));
	}
	
	/// \brief Additionne 2 vecteur
	/// \param v "v" est un vecteur en 3 dimensions.
	/// \return L'opérateur retourne le vecteur modifié.
	Vector3& operator+=(Vector3 v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
	
	/// \brief Additionne 2 vecteur
	/// \param v "v" est un vecteur en 3 dimensions.
	/// \return L'opérateur retourne le vecteur modifié.
	Vector3& operator-=(Vector3 v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}
	
	/// \brief Multiplie les composantes du vecteur pas un nombre.
	/// \param s "s" est un chiffre à virgule.
	/// \return L'opérateur retourne le vecteur modifié.
	Vector3& operator*=(double s) {
		x *= s;
		y *= s;
		z *= s;
		return *this;
	}
	
	/// \brief Divise les composantes du vecteur par un nombre.
	/// \param s "s" est un chiffre à virgule.
	/// \return L'opérateur retourne le vecteur modifié.
	Vector3& operator/=(double s) {
		if(s) {
			x /= s;
			y /= s;
			z /= s;
		}
		return *this;
	}
	
	/// \brief Additionne 2 vecteur
	/// \param v "v" est un vecteur en 3 dimensions.
	/// \return L'opérateur retourne le vecteur modifié.
	inline Vector3 operator+(Vector3 v) {
		return {v.x + x, v.y + y, v.z + z};
	}
	
	/// \brief Soustrait 2 vecteur
	/// \param v "v" est un vecteur en 3 dimensions.
	/// \return L'opérateur retourne le vecteur modifié.
	inline Vector3 operator-(const Vector3& v) {
		return {x - v.x, y - v.y, z - v.z};
	}
	
	/// \brief fait le pro 2 vecteur
	/// \param v "v" est un vecteur en 3 dimensions.
	/// \return L'opérateur retourne un "double".
	inline double operator*(Vector3 v) {
		return (x * v.x) + (y * v.y) + (z * v.z);
	}
	
	/// \brief Multiplie les composantes du vecteur par un nombre.
	/// \param s "s" est un chiffre à virgule.
	/// \return L'opérateur retourne le vecteur modifié.
	inline Vector3 operator* (double s) {
		return Vector3(x * s, y * s, z * s);
	}
	
	/// \brief Effectue le produit vectoriel entre 2 vecteurs.
	/// \param v "v" est un vecteur en 3 dimensions.
	/// \return L'opérateur retourne le vecteur modifié.
	inline Vector3 operator% (Vector3 v) {
		return Vector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
	
	/// \brief Compare deux vecteurs.
	/// \param v Vecteur 3D.
	/// \return Renvoie vrai si les deux vecteurs sont identiques, sinon faux
	inline bool operator==(Vector3 v) {
		return ((x == v.x) && (y == v.y) && (z == v.z));
	}
	
	/// \brief Rend le vecteur unitaire.
	///
	/// Rend la norme du vecteur à 1.
	void Normalize() {
		double norm = GetNorm();
		if (norm) {
			x = x / norm;
			y = y / norm;
			z = z / norm;
		}
	}
	
};
 
#endif
