# SOI2015-opgave_HIER_PUSHEN
Opgave project SOI 2015 van onze groep. Dus hier pushen/mergen.
<b>Indien je vragen of andere opmerkingen hebt, gelieve die dan hier te schrijven bij het gepaste onderwerp.</b>

# Meeting 1:

  Taakverdeling:
  
      Kortste pad algoritme   ->Jorgin
      
      levels bijhouden        ->Domien
      
      game loop & common.c    ->Jens
      
      grafen voorstellen      ->Silke
      
      GUI/Editor.c            ->Jens + Domien
      
  Testen schrijven:
  
      Kortste pad algoritme   ->Domien
      
      levels bijhouden        ->Jens
      
      game loop & common.c    ->Silke
      
      grafen voorstellen      ->Jorgin
  
  Belangerijk:
    
    Genoeg aandacht voor memory leaks
    
    Dijkstra gedeelte 
    
    Mergen naar juiste repo
    
    Geen antwoord 12 uur voor deadline
    
  Testen:
    
    Hoe meer testen hoe beter
    
    Conform voorbeeldtesten
    
    
# Meeting 2  (19/10/2015 : 16h)
    
## Voorbereiding:  
    
    TODO:
    Issue tracker activeren OK
    
    Uitbereidingen kiezen OK
      - Flexibele Level dimensies
      - Extra themes
      - BugFix OK
      - Meer indien tijd
      
    Taakverdeling:
      Level.h OK
      Common.h OK
      Dwgraph
      Game loop/ Editor.c : Grootste deel af, wachten tot dwgraph-header klaar is zodat we weten welke functies er gebruikt kunnen worden.
      GUI functionaliteit OK
    
    Vragen : 
    - Hoe issue tracker gebruiken in Visual studio?
    - level.c:init_level(): nodig voor free(cell)? + level.c:level_free(): voldoende ?
    - Hoe merge conflicts oplossen in VS. OK
    - Errors in C, vb. verkeerd opgestelde file om level-struct van te maken. OK
    - GUI aan level object geraken om vorige level te free'en. OK
    - Verschil tussen binary/text file achterhalen.
    - Kunnen units door elkaar bewegen? met andere woorden: volstaat de level_can_walk_over() functie van level.h 
    - level.h nieuwe functie: updateCell(...), toegelaten? Er is anders te veel code in de game loop voor bv. de headquarters       bij te houden. 

## Verslag:
