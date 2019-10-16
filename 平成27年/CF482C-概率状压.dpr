program CF482C;

{$APPTYPE CONSOLE}

uses
  SysUtils;

const
  maxs = 1 shl 20;

var
  n, m : integer;
  strs : array [1..50] of string;
  d : array [0..maxs] of uint64;
  cof1 : array[0..255] of integer;
  comb : array[0..20]of array[0..20]of extended;

  procedure init;
  var i, j : integer;
  begin
    comb[0, 0] := 1;
    for i := 1 to 20 do begin
      comb[i, 0] := 1;
      for j := 0 to i do begin
        comb[i, j] := comb[i - 1, j] + comb[i - 1, j - 1];
      end;
    end;
    for i := 0 to 255 do begin
      for j := 0 to 7 do begin
        if (i and (1 shl j)) = 0 then
          cof1[i or (1 shl j)] := cof1[i] + 1;
      end;
    end;
  end;

  function GetBit(pos : integer) : uint64;
  begin
	  Result := (uint64(1) shl uint64(pos - 1));
  end;

  function GetCnts(mask : uint64) : integer;
  var i : integer;
  begin
  	result := 0;
    while mask > 0 do begin
      inc(result, cof1[mask and $ff]);
      mask := mask shr 8;
    end;
  end;

  procedure PreProcess;
  var
	  i, j, k : integer;
  	mask : cardinal;
  begin
  	for i := 1 to n do begin
  	  for j := i + 1 to n do begin
  		  mask := 0;
  		  for k := 1 to m do begin
          if strs[i][k] = strs[j][k] then mask := mask or getbit(k);
   		  end;
		    d[mask] := d[mask] or getbit(i) or getbit(j);
	    end;
	  end;
	  for mask := (1 shl m) - 1 downto 0 do begin
	    for j := 1 to m do begin
  	  	if (mask and getbit(j)) > 0 then
		      d[mask xor getbit(j)] := d[mask xor getbit(j)] or d[mask];
	    end;
	  end;
    d[0] := (uint64(1) shl uint64(n)) - 1;
  end;

var
  i, mask, s : integer;
  cnts : array [1..20] of integer;
  ans : extended;

begin
  init;
  readln(n);
  for i := 1 to n do readln(strs[i]);
  if n = 1 then begin
    writeln(0.00);
    exit;
  end;
  m := length(strs[1]);
  PreProcess;
  for mask := 0 to (1 shl m) - 1 do begin
    s := GetCnts(mask);
	  for i := 1 to m do begin
	    inc(cnts[s + 1], GetCnts(d[mask] xor d[mask or getbit(i)]));
  	end;
  end;
  ans := 0;
  for i := 1 to m do begin
	  ans := ans + cnts[i] / comb[m, i];
  end;
  ans := ans / n;
  writeln(ans : 0 : 15);
end.
