import bcrypt
import sys

def crypt(password:str):
    try:
        hash_psw=bcrypt.hashpw(password.encode(),bcrypt.gensalt())
        return hash_psw
    except Exception as ex:
        return bytes(0)

def checkpsw(hash_password:str,password:str):
    if(bcrypt.checkpw(password.encode(),hash_password.encode())):
        return "1"
    else:
        return "0"

if __name__=="__name__":
    if len(sys.argv)>3:
        print("To fue argument")
    elif len(sys.argv)>2:
        if sys.argv[1].lower()=="crypt":
            hash=crypt(sys.argv[2])
            print(str(hash))
        elif sys.argv[1].lower()=="check":
            result=checkpsw(sys.argv[2],sys.argv[3])
            print(result)
        else:
            print("Unknow operation")
    else:
        print("Not enough argument")
    