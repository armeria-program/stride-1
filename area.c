/*
 * Copyright (C) 1992-1994 Dmitrij Frishman <d.frishman at wzw.tum.de>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


#include "stride.h"

void Area(CHAIN **Chain, int NChain, COMMAND *Cmd)
{

  double *Coord, *Radii, OverallArea, *AreaPerAtom, *p1, *p2;
  int At, TotalAt=0, Cn, Res, DotsPerSphere=600;

  for( Cn=0; Cn<NChain; Cn++ ) {

    if( !Chain[Cn]->Valid )
      continue;

    for( Res=0; Res<Chain[Cn]->NRes; Res++ )
      for( At=0; At<Chain[Cn]->Rsd[Res]->NAtom; At++ )
	if( !IsHydrogen(Chain[Cn]->Rsd[Res]->AtomType[At]) )
	  TotalAt ++;
  }

  Coord = (double *)ckalloc(3*TotalAt*sizeof(double));
  Radii = (double *)ckalloc(TotalAt*sizeof(double));

  p1 = Coord;
  p2 = Radii;

  for( Cn=0; Cn<NChain; Cn++ ) {

    if( !Chain[Cn]->Valid )
      continue;

    for( Res=0; Res<Chain[Cn]->NRes; Res++ )
      for( At=0; At<Chain[Cn]->Rsd[Res]->NAtom; At++ ) 
	if( !IsHydrogen(Chain[Cn]->Rsd[Res]->AtomType[At]) ) {
	  (*p1++) = (double)Chain[Cn]->Rsd[Res]->Coord[At][0];
	  (*p1++) = (double)Chain[Cn]->Rsd[Res]->Coord[At][1];
	  (*p1++) = (double)Chain[Cn]->Rsd[Res]->Coord[At][2];
	  (*p2++) = GetAtomRadius(Chain[Cn]->Rsd[Res]->AtomType[At])+1.4;
	}
  
  }  

  p1 = Coord;
  p2 = Radii;

  NSC(Coord,Radii,TotalAt,DotsPerSphere,FLAG_ATOM_AREA,&OverallArea,
      &AreaPerAtom,NULL,NULL,NULL);
    
    for( Cn=0; Cn<NChain; Cn++ ) {

      if( !Chain[Cn]->Valid )
	continue;
      
      for( Res=0; Res<Chain[Cn]->NRes; Res++ ) {
	Chain[Cn]->Rsd[Res]->Prop->Solv = 0.0;
	for( At=0; At<Chain[Cn]->Rsd[Res]->NAtom; At++ )
	  if( !IsHydrogen(Chain[Cn]->Rsd[Res]->AtomType[At]) ) {
	    Chain[Cn]->Rsd[Res]->Prop->Solv += (*AreaPerAtom++);
	  }
      }
    }
  free(Coord);
  free(Radii);
}

    



