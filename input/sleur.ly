\header{
enteredby =	 "jcn";
copyright =	 "PD";
TestedFeatures =	 "This file tests Feta embedded slurs"
	 "(Feta definitively is not an abbreviation of Font-En-TjA)";
}

\version "0.1.9";

shortlong = \melodic{
	c4()c( c c  |
	c c c c |
	c c c c |
	c c c )c |
}

dirs = \melodic {
	\octave c';
	c'1() g' () c () g () c |
}

complex = \melodic{
	\octave c';
	'c16( 'e( 'g( 'b( d( f( a( c'~ )))))))c4 c4 |
	\octave c''';
	'c16( a( f( d( 'b( 'g( 'e( 'c~ )))))))c4 c4 |
}

beum = \melodic{
	\octave c'; 
	[c8 c] c4 c4 c4 |
}

extend = \melodic{
	\octave c;
	c8(( c c )c c c c )c |
	\[4/5c8( c c' c )c\]1/1 c c c c |
	\[4/5c8( c c c c'\]1/1 c c c )c |
	\[4/5c8( c c c c''\]1/1 c c c )c' |
	\[4/5c8( c c c c'\]1/1 c c c )'c |
	\[4/5c8( c c' c c\]1/1 c c c )c |
	\[4/5c8( c c c ''c\]1/1 c c c )c |
	\[4/5c8( c ''c c c\]1/1 c c c )c |
}

extendbug = \melodic{
	\octave c'; 
	c4( c' c' )c
	\stemup c'4( \stemdown e e \stemboth )c'
	[c'8( b g a] [c' d' e' c'] [c' d' e' )c']
	c( c' c' )c
	e( g' g' )e

}

blend = \melodic{
	\octave c; 
	\stemdown; 
%%	    \[4/5c8( c ''c c c\]1/1 c c c )c |
	\[4/5c8( c ''f c c\]1/1 c c c )c |

	\[4/5c8( c \stemup ''f \stemdown c c\]1/1 c c c )c |
	\stemup; 
	\octave c''; 
	\[4/5c8( c f'' c c\]1/1 c c c )c |
	\[4/5c8( c \stemdown f'' \stemup c c\]1/1 c c c )c |
	\stemboth;
	c4( 'c16 'e 'g 'b d f a )'c16 c4 |
	c4( 'c16 'e 'g 'b d f f )'c16 c4 |
	\octave c'; 
	\stemdown; 
	c( c'' c'' )c
	\stemboth;
	e( g' g' )e
	e( g' g' )e
	e( g' g' )e
	e( g' g' )e
}

\score{
	\melodic{ 
		\blend
		\shortlong
		\dirs
		\complex
		\beum
		\octave c'; 
		\stemup; 
		\extend
		\octave c''; 
		\stemdown; 
		\extend
		\stemboth;
		\extendbug
	}
	\paper{ 
%		castingalgorithm = \Wordwrap;
	}
}

