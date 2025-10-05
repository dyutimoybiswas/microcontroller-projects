## Steps

- Manage runtime environment: Select CMSIS:Core
- Options for Target <...>:
	- Choose Stellaris ICDI under Use in Debug tab.
	- C/C++:
		- choose C11 (language C), MISRA compatible (Warnings)
		- add inc folder of project and SW_kit in Include Paths
		- add PART_TM4C123GH6PM in Define