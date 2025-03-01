# ISA 

> **add** reg1, reg2, reg3/imm - сложение с перессылкой.

> **sub** reg1, reg2, reg3/imm - вычитание с перессылкой

> **incbneq** reg1, imm, label - инкремент и переход при не равенстве.

> **jmp** label - переход на метку.

> **call** label - вызов функции.

> **ret** - возврат из функции.

> **shl** reg1, imm - левый сдвиг.

> **cj** reg1, label - переход, если *reg1* == 1.

> **jnpos** reg1, label - переход, если *op1* <= 0 

> **cmpgt/cmplt/cmpeq/cmpgte** reg1, reg2/imm, reg3/imm - сравнить *op1* и *op2* как знаковые числа  
 и сохранить результат в reg1.

> **or** reg1, reg2/imm, reg3/imm - побитовый 'или'.

> **rndrng** reg1, imm - случайное число от 0 до *imm*.

> **slctev** reg1, reg2, reg3/imm, reg4/imm - если *op2* четное, то *op3*, иначе *op4*.

> **rnd** reg1 - случайное число.

> **putpxl** reg1/imm, reg2/imm, reg3/imm - положить пиксель *op3* по координатам x = *op1*, y = *op2*.

> **mul** reg1, reg2/imm, reg3/imm - умножение с перессылкой.

> **mulladd** reg1, reg2, reg3/imm, reg4/imm - умножить *op2* на *op3* и сложить с *op4*.

> **mov** reg1, reg2/imm - перемещение.

> **select** reg1, reg2, reg3/imm, reg4/imm - *op3*, если reg2 == 1, *op4* - иначе

> **flush** - Отобразить кадр.

