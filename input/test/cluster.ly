\version "1.7.4"
\header {
    texidoc = "cluster demo."
}

voiceI = % same as voiceII, but with ordinary notes
	\context Voice = voiceI {
	    \notes \relative c' {
		\stemUp
		c4 f4
		a4 <e4 d'4> | \break
		< g8 a8 > < e8 a8 > a4 c1 < d4 b4 > e4 |
		c4 a4 f4 g4 a4
	    }
	}

voiceII = % same as voiceI, but with cluster notation
	\context Voice = voiceII {
	    \notes \relative c' {
		\property Thread.NoteHead \set #'transparent = ##t
		\property Voice.Stem \set #'transparent = ##t
		\property Voice.Beam \set #'transparent = ##t
		\property Staff.Accidental \set #'transparent = ##t
		\property Voice.Cluster \set #'padding = #0.25
		\property Voice.Cluster \set #'shape = #'ramp
		c4 f4
		\startCluster
		a4 <e4 d'4> | \break
		%%% do not try something like: < { g8 e8 } a4 >
		%%% instead, do the following: < g8 a8 > < e8 a8 >
		< g8 a8 > < e8 a8 > a4 c1 < d4 b4 > e4 |
		c4 \stopCluster a4 f4 g4 a4
	    }
	}

\score {
	\context PianoStaff {
	<
		\voiceI
		\voiceII
	>
	}
	\paper{
		linewidth = 15.0 \cm
	}
}
