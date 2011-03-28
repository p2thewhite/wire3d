//----------------------------------------------------------------------------
template <typename Real>
Real NoisePerlin<Real>::Noise(Real x, Real y, Real z) 
{
	Int X = static_cast<Int>(Math<Real>::Floor(x)) & 255; /* FIND CUBE THAT */
	Int	Y = static_cast<Int>(Math<Real>::Floor(y)) & 255; /* CONTAINS POINT.*/
	Int	Z = static_cast<Int>(Math<Real>::Floor(z)) & 255;
	x -= Math<Real>::Floor(x);                       /* FIND RELATIVE X,Y,Z */
	y -= Math<Real>::Floor(y);                       /* OF POINT IN CUBE.   */
	z -= Math<Real>::Floor(z);
	Real u = Fade(x);                                /* COMPUTE FADE CURVES */
	Real v = Fade(y);                                /* FOR EACH OF X,Y,Z.  */
	Real w = Fade(z);
	Int A = smP[X] + Y; 
	Int	AA = smP[A] + Z;
	Int AB = smP[A+1] + Z;					         /* HASH COORDINATES OF */
	Int B = smP[X+1] + Y; 
	Int BA = smP[B] + Z;
	Int BB = smP[B+1] + Z;					         /* THE 8 CUBE CORNERS, */

	return Lerp(w, Lerp(v, Lerp(u, Grad(smP[AA], x, y, z),       /* AND ADD */
                                   Grad(smP[BA], x-1, y, z)),    /* BLENDED */
                           Lerp(u, Grad(smP[AB], x, y-1, z),     /* RESULTS */
                                   Grad(smP[BB], x-1, y-1, z))), /* FROM  8 */
                   Lerp(v, Lerp(u, Grad(smP[AA+1], x, y, z-1 ),  /* CORNERS */
                                   Grad(smP[BA+1], x-1, y, z-1)),/* OF CUBE */
                           Lerp(u, Grad(smP[AB+1], x, y-1, z-1),
                                   Grad(smP[BB+1], x-1, y-1, z-1))));
}

//----------------------------------------------------------------------------
template <typename Real>
Real NoisePerlin<Real>::Fade(Real t)
{
	return t * t * t * (t * (t * 6 - 15) + 10);
}

//----------------------------------------------------------------------------
template <typename Real>
Real NoisePerlin<Real>::Lerp(Real t, Real a, Real b)
{
	return a + t * (b - a);
}

//----------------------------------------------------------------------------
template <typename Real>
Real NoisePerlin<Real>::Grad(Int hash, Real x, Real y, Real z) 
{
	Int h = hash & 15;                    /* CONVERT LO 4 BITS OF HASH CODE */
	Real u = h < 8 ? x : y;               /* INTO 12 GRADIENT DIRECTIONS.   */
	Real v = h < 4 ? y : h==12||h==14 ? x : z;
	return ((h&1) == 0 ? u : -u) + ((h&2) == 0 ? v : -v);
}
