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

Sub NewbieCollector
  If NumRings >= 20 And PlayerActive Then
    Award(New Trophy With {
      .Title = "Newbie Collector",
      .Description = "Collect 20 Rings",
      .BadgeName = "00205",
      .Points = 5
    })
  End If
End Sub

Sub RingCollector
  If NumRings >= 100 And PlayerActive Then
    Award(New Trophy With {
      .Title = "Ring Collector",
      .Description = "Collect 100 Rings!",
      .BadgeName = "00206",
      .Points = 10
    })
  End If
End Sub

Sub ProCollector
  If NumRings >= 200 And PlayerActive Then
    Award(New Trophy With {
      .Title = "Pro Collector",
      .Description = "Collect 200 Rings!!",
      .BadgeName = "00207",
      .Points = 20
    })
  End If
End Sub

Sub ThatWasEasy
  Static Dim CurrentActLast As Uint8 = 0
  Dim CurrentActNow As Uint8 = CurrentAct
  
  If (CurrentActNow - CurrentActLast) = 1 And PlayerActive Then
    Award(New Trophy With {
      .Title = "That Was Easy",
      .Description = "Complete the first act in Green Hill Zone",
      .BadgeName = "00208",
      .Points = 5
    })
  End If
  
  CurrentActLast = CurrentActNow
End Sub

Sub TheFloorIsLava
  If CurrentAct = 2 And PlayerActive Then
    Award(New Trophy With {
      .Title = "The Floor Is Lava",
      .Description = "Get to Marble Zone!",
      .BadgeName = "00209",
      .Points = 10
    })
  End If
End Sub
```
