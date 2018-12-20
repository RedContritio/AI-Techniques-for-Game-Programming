#ifndef __GENETIC__ALGORITHM__INC
#define __GENETIC__ALGORITHM__INC
#include <cassert>

#include <vector>
#include <algorithm>
using std::vector ;


namespace RedContritio
{
	const double FITNESS_MAX = 99999999 ;
	
	template<typename gene_type>
	struct SGENOME
	{
		vector<gene_type> m_genes ;
		double m_fitness ;
		SGENOME(void ):m_fitness(0 ){}
		SGENOME(vector<gene_type> chromo ,double fitness ):m_genes(chromo ),m_fitness(fitness ){}
		static void (*Initialize )(size_t ChromoLength );
		bool operator<(const SGENOME<gene_type>& rhs)
		{
			return this->m_fitness < rhs.m_fitness ;
		}
		bool operator>(const SGENOME<gene_type>& rhs)
		{
			return this->m_fitness > rhs.m_fitness ;
		}
	};

	
	template<typename gene_type>
	void (*(SGENOME<gene_type>::Initialize) )(size_t ) = NULL ;
	

	template<typename gene_type>
	class GeneticAlgorithm
	{
	private :
		vector<SGENOME<gene_type>> m_Population ;
		size_t m_PopulationSize ;
		size_t m_ChromoLength ;

		double m_TotalFitness ;
		double m_BestFitness ;
		double m_AverageFitness ;
		double m_WorstFitness ;

		int m_FittestGenome ;

		double m_MutationRate ;
		double m_CrossoverRate ;

		int m_Generation ;

		unsigned m_MethodRegistration ;
		
		void (*Crossover )(const vector<gene_type>& ,const vector<gene_type>& ,vector<gene_type>& ,vector<gene_type>& );
		void (*Mutate )(vector<gene_type>& );

		SGENOME<gene_type> (*SelectParent )(const vector<SGENOME<gene_type>>& );

		void GrabNBest(int NBest ,const int NumCopies ,vector<SGENOME<gene_type>>& Population )
		{
			while(NBest-- )
			{
				for(int i=0 ;i<NumCopies ;i++ )
				{
					Population.push_back(m_Population[m_PopulationSize-NBest-1] );
				}
			}
		}

		void (*FitnessScaleRank )(vector<gene_type>& );

		void CalculateRelatedFitness(void )
		{
			Reset();
			for(size_t i=0 ;i<m_PopulationSize ;i++ )
			{
				if(m_Population[i].m_fitness > m_BestFitness )
				{
					m_BestFitness = m_Population[i].m_fitness ;
					m_FittestGenome = i ;
				}
				if(m_Population[i].m_fitness < m_WorstFitness )
				{
					m_WorstFitness = m_Population[i].m_fitness ;
				}
				m_TotalFitness += m_Population[i].m_fitness ;
			}
			m_AverageFitness = m_TotalFitness / m_PopulationSize ;
		}
		void Reset(void )
		{
			m_TotalFitness = 0 ;
			m_BestFitness = 0 ;
			m_WorstFitness = FITNESS_MAX ;
			m_AverageFitness = 0 ;
			return ;
		}
	public :
		GeneticAlgorithm(size_t PopulationSize ,double CrossoverRate ,double MutationRate ,size_t ChromoLength )
		:m_PopulationSize(PopulationSize ),m_CrossoverRate(CrossoverRate ),m_MutationRate(MutationRate ),m_ChromoLength(ChromoLength ),
		 m_Generation(0 ),m_FittestGenome(0 ),m_MethodRegistration(0x00 )
		{
			Reset();
			for(size_t i=0 ;i<m_PopulationSize ;i++ )
			{
				m_Population.push_back(SGENOME<gene_type>());
				if(m_Population[i].Initialize )m_Population[i].Initialize(m_ChromoLength );
			}
		}

		vector<SGENOME<gene_type>> Epoch(vector<SGENOME<gene_type>>& PrevPopulation )
		{
			m_Population = PrevPopulation ;
			
			Reset();

			std::sort(m_Population.begin() ,m_Population.end() );

			CalculateRelatedFitness();

			vector<SGENOME<gene_type>> NextPopulation ;

			assert(m_MethodRegistration == 0x07 );
			//TODO : GrabNBest 

			while(NextPopulation.size() < m_PopulationSize )
			{
				SGENOME<gene_type> parent1 = SelectParent(m_Population );
				SGENOME<gene_type> parent2 = SelectParent(m_Population );

				vector<gene_type> child1 ,child2 ;

				Crossover(parent1.m_genes ,parent2.m_genes ,child1 ,child2 );

				Mutate(child1 );
				Mutate(child2 );

				NextPopulation.push_back(SGENOME<gene_type>(child1 ,0 ));
				NextPopulation.push_back(SGENOME<gene_type>(child2 ,0 ));
			}

			m_Population = NextPopulation ;

			return m_Population ;
		}

		vector<SGENOME<gene_type>> GetChromos(void )const {return m_Population ;}

		double AverageFitness(void )const {return m_AverageFitness ;}
		double BestFitness(void )const {return m_BestFitness ;}

		void SetCrossoverMethod(void (*method )(const vector<gene_type>& ,const vector<gene_type>& ,vector<gene_type>& ,vector<gene_type>& ))
		{
			if(method )m_MethodRegistration |= 0x01 ;
			else m_MethodRegistration &= ~(unsigned )0x01 ;
			Crossover = method ;
		}
		void SetMutateMethod(void (*method )(const vector<gene_type>& ,const vector<gene_type>& ,vector<gene_type>& ,vector<gene_type>& ))
		{
			if(method )m_MethodRegistration |= 0x02 ;
			else m_MethodRegistration &= ~(unsigned )0x02 ;
			Mutate = method ;
		}
		void SetSelectMethod(SGENOME<gene_type> (*method )(const vector<SGENOME<gene_type>>& ))
		{
			if(method )m_MethodRegistration |= 0x04 ;
			else m_MethodRegistration &= ~(unsigned )0x04 ;
			SelectParent = method ;
		}

		void (*GetCrossoverMethod(void ))(const vector<gene_type>& ,const vector<gene_type>& ,vector<gene_type>& ,vector<gene_type>& )
		{
			return Crossover ;
		}
		void (*GetMutateMethod(void ))(const vector<gene_type>& ,const vector<gene_type>& ,vector<gene_type>& ,vector<gene_type>& )
		{
			return Mutate ;
		}
		void (*GetSelectMethod(void ))(const vector<SGENOME<gene_type>>& )
		{
			return SelectParent ;
		}
	};
}

#endif
