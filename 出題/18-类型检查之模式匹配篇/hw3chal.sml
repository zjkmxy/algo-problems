(* Coursera Programming Languages, Homework 3, Challenge Problem *)

(*
Write a function typecheck_patterns that "type-checks" a pattern list.
Patterns and types are defined follow.

typecheck_patterns should have type ((string * string * typ) list) * (pattern list) -> typ option.
The first argument contains elements that look like ("foo","bar",IntT), which means constructor foo
makes a value of type Datatype "bar" given a value of type IntT. Assume list elements all have different
first fields (the constructor name), but there are probably elements with the same second field (the datatype
name). Under the assumptions this list provides, you "type-check" the pattern list to see if there exists
some typ (call it t) that all the patterns in the list can have. If so, return SOME t, else return NONE.

You must return the "most lenient" type that all the patterns can have. For example, given patterns
TupleP[Variable("x"),Variable("y")] and TupleP[Wildcard,Wildcard], return TupleT[Anything,Anything]
even though they could both have type TupleT[IntT,IntT]. As another example, if the only patterns
are TupleP[Wildcard,Wildcard] and TupleP[Wildcard,TupleP[Wildcard,Wildcard]], you must return
TupleT[Anything,TupleT[Anything,Anything]].
*)

(* Provided Code *)

exception NoAnswer

datatype pattern = Wildcard
		 | Variable of string
		 | UnitP
		 | ConstP of int
		 | TupleP of pattern list
		 | ConstructorP of string * pattern

datatype typ = Anything
	     | UnitT
	     | IntT
	     | TupleT of typ list
	     | Datatype of string

(**** you can put all your code here ****)

fun first_answer f ls =
    case ls of
        [] => raise NoAnswer
     |  x::ls => case f x of
                     NONE => first_answer f ls
		  |  SOME v => v

fun typecheck_patterns (dcl, pl) =
    let
	fun findup s = first_answer (fn (s1, s2, t) => if s = s1 then SOME (Datatype s2, t) else NONE) dcl
	fun get_type (Wildcard, acc) = acc
	  | get_type (Variable _, acc) = acc
	  | get_type (UnitP, Anything) = UnitT
	  | get_type (UnitP, UnitT) = UnitT
	  | get_type (ConstP _, Anything) = IntT
	  | get_type (ConstP _, IntT) = IntT
	  | get_type (TupleP ps, Anything) = TupleT (List.map (fn x => get_type (x, Anything)) ps)
	  | get_type (TupleP ps, TupleT ts) = TupleT (List.map get_type (ListPair.zipEq (ps, ts)))
	  | get_type (ConstructorP (s1, p), Anything) = let
		val (ret, tp) = findup s1
		val _ = get_type (p, tp)
	    in ret end
	  | get_type (ConstructorP (s1, p), Datatype s2) = let
	      val (Datatype ret, tp) = findup s1
	      val _ = get_type (p, tp)
	    in if ret = s2 then Datatype ret else raise NoAnswer end
	  | get_type (_, _) = raise NoAnswer
    in
	SOME (List.foldl get_type Anything pl) handle _ => NONE
    end