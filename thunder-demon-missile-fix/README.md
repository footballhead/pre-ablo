Thunder Demo Missile Fix
===============================================================================

(apply after infraring_fix)

The Thunder Demon missile calls GetSpellLevel (.text:00453CDB) with a monster ID. This is supposed to be a player ID though, so it can crash trying to access an invalid entry in the `plr` table.

Retail added ID validation, we can try the same.

```cxx
if (id != myplr) {
	return 1;
}
// Rest of GetSpellLevel
```





```
mov eax, [ebp+id]
mov ebx, [myplr]
cmp eax, ebx
jz normal_function_body
mov eax, 1
jmp end_of_function
```


```
.text:00401129
...
.text:00436D65

```