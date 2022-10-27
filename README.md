my-crypto
===
1. Testing sha256 api
2. Practice of api learning before

# how to use?
## Module part
1. `make`

## Userspace part
### 1. insmod and set password
`sudo ./execute.sh 1 [password you guess]`
```
ben@ben-OptiPlex-7060:/working/ben/module/my-crypto$ sudo ./execute.sh 1 123
[my_crypto]: Enter my_crypto_init, hello
[my_crypto]: Please printf the password in /proc/guess_password (do not using echo!)
[my_crypto]: The sha256 of the password is:
[my_crypto]: a665a45920422f9d417e4867efdc4fb8a04a1f3fff1fa07e998e86f7f7a27ae3
```

### Userspace part
`printf [guessword] > /proc/guess_password`
- success
```
ben@ben-OptiPlex-7060:/working/ben/module/my-crypto$ printf 000 > /proc/guess_password
[my_crypto]: Checking sha256...
[my_crypto]: .
[my_crypto]: .
[my_crypto]: .
[my_crypto]: .
[my_crypto]: Sorry! Sha256 comapare not match!
[my_crypto]: Ths sha256 of your answer is:
[my_crypto]: 2ac9a6746aca543af8dff39894cfe8173afba21eb01c6fae33d52947222855ef
```
- fail
```
ben@ben-OptiPlex-7060:/working/ben/module/my-crypto$ printf 123 > /proc/guess_password
[my_crypto]: Checking sha256...
[my_crypto]: .
[my_crypto]: .
[my_crypto]: .
[my_crypto]: .
[my_crypto]: WoW! Sha256 comapare match! Checking password next...
[my_crypto]: .
[my_crypto]: .
[my_crypto]: .
[my_crypto]: .
[my_crypto]: Congratulations! Password match!
```
:::warning
Suggest not to use echo. Because 'echo 123' will be '123\n' in string 
ex:
1. echo 123 > 123
2. vim 123
3. :%!xxd
```
00000000: 3132 330a                                123.
```
4. you can see the
31 = 1
32 = 2
33 = 3
0a = Line Feed
::

### 3. rmmod
`sudo ./execute.sh 0`
```
ben@ben-OptiPlex-7060:/working/ben/module/my-crypto$ sudo ./execute.sh 0
[my_crypto]: Enter my_crypto_exit, bye bye
```

## Description
Key point
1. Module parameter
2. module print to tty
3. proc entry
4. sha256
5. atomic
6. completion
7. workqueue

