import uuid
import bcrypt

def generate_code():
    return str(uuid.uuid4())

if __name__=="__main__":
    password=bcrypt.hashpw("jifrjf".encode(),bcrypt.gensalt())
    print(type(password))