#include "CMinesweeper.h"

//-----------------------------------constructor-------------------------
//
//-----------------------------------------------------------------------
CMinesweeper::CMinesweeper():
                             m_dRotation(2*RandFloat()*CParam::Pi),
                             m_lTrack(0.16),
                             m_rTrack(0.16),
                             m_dFitness(CParam::StartEnergy),
							               m_dScale(CParam::SweeperScale),
                             m_iClosestMine(0)
			 
{
	//create a random start position
	m_vPosition = S2DVector((RandFloat() * CParam::WindowWidth), 
					                (RandFloat() * CParam::WindowHeight));
  
}

//-------------------------------------------Reset()--------------------
//
//	Resets the sweepers position, energy level and rotation
//
//----------------------------------------------------------------------
void CMinesweeper::Reset()
{
	//reset the sweepers positions
	m_vPosition = S2DVector((RandFloat() * CParam::WindowWidth), 
					                (RandFloat() * CParam::WindowHeight));
	
	//and the energy level
	m_dFitness = CParam::StartEnergy;

  //and the rotation
  m_dRotation = 2*RandFloat()*CParam::Pi;

	return;
}
//---------------------WorldTransform--------------------------------
//
//	sets up a translation matrix for the sweeper according to its
//  scale, rotation and position. Returns the transformed vertices.
//-------------------------------------------------------------------
void CMinesweeper::WorldTransform(vector<SPoint> &sweeper)
{
	//create the world transformation matrix
	C2DMATRIX::TransformSPoints(sweeper ,m_vPosition.x ,m_vPosition.y ,m_dScale ,m_dScale ,m_dRotation );
}

//-------------------------------Update()--------------------------------
//
//	First we take sensor readings and feed these into the sweepers brain.
//
//	The inputs are:
//	
//  a signed angle to the closest mine
//
//	We receive two outputs from the brain.. lTrack & rTrack.
//	So given a force for each track we calculate the resultant rotation 
//	and acceleration and apply to current velocity vector.
//
//-----------------------------------------------------------------------
bool CMinesweeper::Update(vector<S2DVector> &mines)
{
	
	//this will store all the inputs for the NN
	vector<double> inputs;	

	//get vector to closest mine
	S2DVector vClosestMine = GetClosestMine(mines);
  
	//normalize it
	vClosestMine.Normalize();
  
  //calculate dot product of the look at vector and Closest mine
  //vector. This will give us the angle we need turn to face
  //the closest mine
	double dot =m_vLookAt *vClosestMine ;
//  double dot = Vec2DDot(m_vLookAt, vClosestMine);

  //calculate sign
	int sign   = S2DVSign(m_vLookAt, vClosestMine);

	inputs.push_back(dot*sign);

  
	//update the brain and get feedback
	vector<double> output = m_ItsBrain.Update(inputs);

	//make sure there were no errors in calculating the 
	//output
	if (output.size() < CParam::NumOfOutput ) 
  {
    return false;
  }

	//assign the outputs to the sweepers left & right tracks
	m_lTrack = output[0];
	m_rTrack = output[1];

	//calculate steering forces
	double RotForce = m_lTrack - m_rTrack;

	//clamp rotation
	Clamp(RotForce, -CParam::MaxTurnRate, CParam::MaxTurnRate);

	m_dRotation += RotForce;

  m_dSpeed = (m_lTrack + m_rTrack);

	//update Look At 
	m_vLookAt.x = -sin(m_dRotation);
	m_vLookAt.y = cos(m_dRotation);

	//update position
  m_vPosition += (m_vLookAt * m_dSpeed);

	//wrap around window limits
	if (m_vPosition.x > CParam::WindowWidth) m_vPosition.x = 0;
	if (m_vPosition.x < 0) m_vPosition.x = CParam::WindowWidth;
	if (m_vPosition.y > CParam::WindowHeight) m_vPosition.y = 0;
	if (m_vPosition.y < 0) m_vPosition.y = CParam::WindowHeight;

	return true;
}


//----------------------GetClosestObject()---------------------------------
//
//	returns the vector from the sweeper to the closest mine
//
//-----------------------------------------------------------------------
S2DVector CMinesweeper::GetClosestMine(vector<S2DVector> &mines)
{
	double			closest_so_far = 99999;

	S2DVector		vClosestObject(0, 0);

	//cycle through mines to find closest
	for (int i=0; i<mines.size(); i++)
	{
		double len_to_object = (mines[i] - m_vPosition).Length();

		if (len_to_object < closest_so_far)
		{
			closest_so_far	= len_to_object;
			
			vClosestObject	= m_vPosition - mines[i];

      m_iClosestMine = i;
		}
	}

	return vClosestObject;
}
//----------------------------- CheckForMine -----------------------------
//
//  this function checks for collision with its closest mine (calculated
//  earlier and stored in m_iClosestMine)
//-----------------------------------------------------------------------
int CMinesweeper::CheckForMine(vector<S2DVector> &mines, double size)
{
  S2DVector DistToObject = m_vPosition - mines[m_iClosestMine];
		
	if ((DistToObject.Length()) < (size + 5))
	{
			return m_iClosestMine;
  }

  return -1;
}

		
