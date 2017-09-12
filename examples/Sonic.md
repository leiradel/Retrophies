Some [Retro Achievements](http://retroachievements.org/) achievements for [Sonic](http://retroachievements.org/game/1) translated to Retrophies.


```vbnet
Function NumRings As Uint16
  Return Ram.Uint16.LE[&hFE20]
End Function

Function CurrentAct As Uint8
  Return Ram.Uint8[&hFE10]
End Function

Function DemoMode As Boolean
  Return Ram.Uint16.LE[&hFFF0] <> 0
End Function

Function LevelSelect As Boolean
  Return Ram.Uint16.LE[&hFFE1] <> 0
End Function

Function DebugMode As Boolean
  Return Ram.Uint8[&hFFFB] <> 0
End Function

Function PlayerActive As Boolean
  Return Not DemoMode And Not LevelSelect And Not DebugMode
End Function

Trophy NewbieCollector
  If NumRings >= 20 And PlayerActive Then
    Award("Newbie Collector", "Collect 20 Rings", "00205", 5)
  End If
End Trophy

Trophy RingCollector
  If NumRings >= 100 And PlayerActive Then
    Award("Ring Collector", "Collect 100 Rings!", "00206", 10)
  End If
End Trophy

Trophy ProCollector
  If NumRings >= 200 And PlayerActive Then
    Award("Pro Collector", "Collect 200 Rings!!", "00207", 20)
  End If
End Trophy

Trophy ThatWasEasy
  Static Dim CurrentActLast As Uint8 = 0
  Dim CurrentActNow As Uint8 = CurrentAct
  
  If (CurrentActNow - CurrentActLast) = 1 And PlayerActive Then
    Award("That Was Easy", "Complete the first act in Green Hill Zone", "00208", 5)
  End If
  
  CurrentActLast = CurrentActNow
End Trophy

Trophy TheFloorIsLava
  If CurrentAct = 2 And PlayerActive Then
    Award("The Floor Is Lava", "Get to Marble Zone!", "00209", 10)
  End If
End Trophy

Sub TestTrophies Handles OnFrame
  NewbieCollector()
  RingCollector()
  ProCollector()
  ThatWasEasy()
  TheFloorIsLava()
End Sub
```
