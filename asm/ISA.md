# ISA 

> **add** reg1, reg2/imm, reg3/imm - сложение с перессылкой.

> **sub** reg1, reg2/imm, reg3/imm - вычитание с перессылкой

> **shl** reg1, reg2, imm - левый сдвиг.

> **cmpgt/cmplt/cmpneq/cmpgte** reg1, reg2/imm, reg3/imm - сравнить *op1* и *op2* как знаковые числа  
 и сохранить результат в reg1.

> **or** reg1, reg2/imm, reg3/imm - побитовый 'или'.

> **and** reg1, reg2/imm, reg3/imm - побитовый 'и'.

> **slctev** reg1, reg2, reg3/imm, reg4/imm - если *op2* четное, то *op3*, иначе *op4*.

> **mul** reg1, reg2/imm, reg3/imm - умножение с перессылкой.

> **mulladd** reg1, reg2, reg3/imm, reg4/imm - умножить *op2* на *op3* и сложить с *op4*.

> **mov** reg1, reg2/imm - перемещение.

> **select** reg1, reg2, reg3/imm, reg4/imm - *op3*, если reg2 == 1, *op4* - иначе


## Sim instructions

> **flush** - Отобразить кадр.

> **rnd** reg1 - случайное число.

> **putpxl** reg1/imm, reg2/imm, reg3/imm - положить пиксель *op3* по координатам x = *op1*, y = *op2*.

> **rndrng** reg1, imm - случайное число от 0 до *imm*.


## Control flow instructions

> **incjneq** reg1, imm, label - инкремент и переход при не равенстве.

> **jmp** label - переход на метку.

> **call** label - вызов функции.

> **cj** reg1, label - переход, если *reg1* == 1.

> **jnpos** reg1, label - переход, если *op1* <= 0 

> **ret** - возврат из функции.