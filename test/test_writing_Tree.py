import ROOT

o = ROOT.TFile( 'myfile.root', 'RECREATE' )

t = ROOT.TTree( 'mytree', 'My Tree' )
v = ROOT.std.vector( ROOT.std.string )()
t._v = v
t.Branch( 'mystrings', v )

myStrings = [ 'aap', 'noot', 'mies', 'zus', 'jet' ]

for s in myStrings:
   v.push_back( s )
   t.Fill()

t.Write()
