import bcrypt
import sys

def crypt(password:str):
    try:
        hash_psw=bcrypt.hashpw(password.encode(),bcrypt.gensalt())
        return hash_psw.decode()
    except Exception as ex:
        return "0"

def checkpsw(hash_password:str,password:str):
    if(bcrypt.checkpw(password.encode(),hash_password.encode())):
        return "0"
    else:
        return "1"

if __name__=="__main__":
    if sys.argv[1].lower()=="crypt":
        hash=crypt(sys.argv[2])
        print(str(hash))
    elif sys.argv[1].lower()=="check":
        result=checkpsw(sys.argv[2],sys.argv[3])
        print(result)
    else:
            print("Unknow operation")
