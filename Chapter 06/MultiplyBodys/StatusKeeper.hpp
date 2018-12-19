#ifndef __STATEKEEPER__RC__INC
#define __STATEKEEPER__RC__INC

#pragma once
namespace RedContritio 
{
	template <typename T >
	class StatusKeeper
	{
		private :
			T Memory ;
			bool isSaved ;
		public :
			StatusKeeper(void )
			{
				isSaved = false ;
			}
			void Save(const T &content )
			{
				Memory = content ;
				isSaved = true ;
			}
			const T &Load(void )const
			{
				if(isSaved )return Memory ;
				else return Memory ;
			}
			void Clear(void )
			{
				isSaved = false ;
			}
			bool Saved(void )
			{
				return isSaved ;
			}
	};
}
#endif