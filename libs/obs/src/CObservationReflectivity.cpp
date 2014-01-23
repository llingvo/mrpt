/* +---------------------------------------------------------------------------+
   |                     Mobile Robot Programming Toolkit (MRPT)               |
   |                          http://www.mrpt.org/                             |
   |                                                                           |
   | Copyright (c) 2005-2014, Individual contributors, see AUTHORS file        |
   | See: http://www.mrpt.org/Authors - All rights reserved.                   |
   | Released under BSD License. See details in http://www.mrpt.org/License    |
   +---------------------------------------------------------------------------+ */

#include <mrpt/obs.h>   // Precompiled headers

#include <mrpt/slam/CObservationReflectivity.h>

using namespace mrpt::slam; 
using namespace mrpt::utils; 
using namespace mrpt::poses;

// This must be added to any CSerializable class implementation file.
IMPLEMENTS_SERIALIZABLE(CObservationReflectivity, CObservation,mrpt::slam)


/** Default constructor.
 */
CObservationReflectivity::CObservationReflectivity( ) :
	reflectivityLevel ( 0.5f ),
	sensorPose(),
	sensorStdNoise( 0.2f )
{
}

CObservationReflectivity::~CObservationReflectivity()
{
}

/*---------------------------------------------------------------
  Implements the writing to a CStream capability of CSerializable objects
 ---------------------------------------------------------------*/
void  CObservationReflectivity::writeToStream(CStream &out, int *version) const
{
	if (version)
		*version = 0;
	else
	{
		out << reflectivityLevel << sensorPose;
		out << sensorLabel
			<< timestamp;
	}
}

/*---------------------------------------------------------------
  Implements the reading from a CStream capability of CSerializable objects
 ---------------------------------------------------------------*/
void  CObservationReflectivity::readFromStream(CStream &in, int version)
{
	switch(version)
	{
	case 0:
		{
			in >> reflectivityLevel >> sensorPose;
			in >> sensorLabel
			   >> timestamp;
		} break;
	default:
		MRPT_THROW_UNKNOWN_SERIALIZATION_VERSION(version)
	};
}
