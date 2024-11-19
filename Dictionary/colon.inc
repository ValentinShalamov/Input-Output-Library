; This macros defines the entries of linked list
; %1 is a null terminated string, %2 is the internal label of the string

%define next_entry 0

%macro colon 2
	%ifstr %1
		%ifid %2
			%%next_entry: dq next_entry
			%2:
			db %1, 0
			%define next_entry %%next_entry
		%else
			%error "The second argument must be a pointer"
		%endif
	%else
		%error "The key must be inside quotes"
	%endif
%endmacro
