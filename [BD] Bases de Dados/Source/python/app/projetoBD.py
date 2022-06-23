##
## =============================================
## ============== Bases de Dados ===============
## ============== LEI  2020/2021 ===============
## =============================================
## =================== Demo ====================
## =============================================
## =============================================
## === Department of Informatics Engineering ===
## =========== University of Coimbra ===========
## =============================================
##
## Authors: 
##   Davide Figueiredo Areias <2019219422>
##	 Rui Eduardo Carvalho Marques <2019216539>
## 	 Thomas Pereira Fresco <2019219057>
##   University of Coimbra

 
from flask import Flask, jsonify, request, render_template
import logging, psycopg2, time
from datetime import date
from  werkzeug.security import generate_password_hash, check_password_hash
# imports for PyJWT authentication
import jwt
from datetime import datetime, timedelta
from functools import wraps

app = Flask(__name__)

@app.route('/dbproj/')
def hello(): 
	return render_template("index.html")


##########################################################
## FUNCTIONS
##########################################################

def token_required(f):
	@wraps(f)
	def decorated(*args, **kwargs):
		token = None
		if 'Authorization' in request.headers:
			token = request.headers['Authorization']
		if not token:
			return jsonify({'message': 'Token is missing'})
		if 'Bearer' in token:
			#logger.info(token)
			temp = token.replace('Bearer ', '')
			#logger.info(temp)
			token = temp

		conn = db_connection()
		cur = conn.cursor()
		logger.info(token)
		logger.info(type(token))
		try:
			logger.info("HERE")
			data = jwt.decode(token, app.config['SECRET_KEY'], algorithms=["HS256"])
			logger.info(data)
			statements = """SELECT id FROM utilizador WHERE email = %s AND status = TRUE"""
			values = (data['public_id'],)
			#cur.execute("Begin transaction") #a tabela esta em ACESS EXCLUSIVE
			cur.execute(statements, values)
			current_user = cur.fetchall()
			#cur.execute("commit")
			logger.info(current_user)
			conn.close()
			if not current_user:
				return jsonify({'message': 'Token is wrong'})
		except:
			conn.close()
			return jsonify({'message': 'Token is invalid'})
		return f(current_user[0][0], *args, **kwargs)
	return decorated

def verifyPasswordAndReturnToken(password, email):
	passwordEnc = getPassword(email)
	if passwordEnc is None:
		return -2

	if check_password_hash(passwordEnc, password):
		token = jwt.encode({
			'public_id': email,
			'exp' : datetime.utcnow() + timedelta(minutes = 30)
		}, app.config['SECRET_KEY'])
		logger.info(type(token))
		return token
	else:
		return -1

def add_description(ean, description):
	logger.info("###              ADD DESCRIPTION FUNCTION             ###");   


	conn = db_connection()
	cur = conn.cursor()

	# parameterized queries, good for security and performance
	statement = """
		INSERT INTO descricao (description, currentdescription, modifieddate, leilao_ean)
		VALUES (%s, TRUE, CURRENT_TIMESTAMP, %s)
		"""

	values = (
		description,
		ean
	)
	
	try:
		#cur.execute("Begin transaction")
		cur.execute(statement, values)
		cur.execute('commit')
		result = 'New Description Inserted!!'
	except (Exception, psycopg2.DatabaseError) as error:
		logger.error(error)
		result = 'Failed to insert New description!'
	finally:
		if conn is not None:
			conn.close()

	return jsonify(result)


def update_price(leilaoId, licitacao):
	logger.info("###              Price Update FUNCTION              ###");   

	conn = db_connection()
	cur = conn.cursor()

	statement ="""
				UPDATE leilao 
				SET price = %s
				WHERE ean = %s AND %s > leilao.price
				"""

	values = (
		licitacao,
		leilaoId,
		licitacao
	)

	try:
		#cur.execute("Begin transaction")
		cur.execute(statement, values)
		cur.execute("commit")
		result = 'Price Updated!'
	except (Exception, psycopg2.DatabaseError) as error:
		logger.error(error)
		result = 'Failed to update price!'
	finally:
		if conn is not None:
			conn.close()

	return jsonify(result)


def isFinished(ean):
	conn = db_connection()
	cur = conn.cursor()

	statement = """
		SELECT state
		FROM leilao WHERE ean = %s
		"""
	values = (
		ean,
		)
	#cur.execute("Begin transaction")
	#cur.execute("LOCk table leilao in EXCLUSIVE MODE")
	cur.execute(statement, values)
	result = cur.fetchall()
	#cur.execute("commit")
	conn.close()
	return result[0][0]


def isOwner(user_id, ean):
	conn = db_connection()
	cur = conn.cursor()

	statement ="""
				SELECT utilizador_id FROM leilao WHERE ean = %s
				"""

	values = (
		ean,
	)

	#cur.execute("Begin transaction")
	#cur.execute("LOCk table leilao in EXCLUSIVE MODE")
	cur.execute(statement, values)
	userIdOwner = cur.fetchall()
	#cur.execute("commit")
	#logger.error(userIdOwner)
	#logger.info(user_id)
	#logger.info(userIdOwner[0][0])
	#logger.info(str(type(userIdOwner[0][0]))+' '+ str(type(user_id)))
	if not userIdOwner:
		conn.close()
		return -2 #leilao nao encontrado
	if userIdOwner[0][0] != int(user_id):
		conn.close()
		return -1 #nao e o dono
	else:
		conn.close()
		return 0 #e o dono

def getPassword(email):
	conn = db_connection()
	cur = conn.cursor()

	statement ="""
				SELECT password FROM utilizador WHERE email = %s AND status = TRUE
				"""

	values = (
		email,
	)

	#cur.execute("Begin transaction")
	#cur.execute("LOCk table utilizador in EXCLUSIVE MODE")
	cur.execute(statement, values)
	password = cur.fetchall()
	#cur.execute("commit")
	conn.close()
	if not password:
		return None
	return password[0][0]

def isAdmin(user_id):
	conn = db_connection()
	cur = conn.cursor()

	statement = """
		SELECT admin FROM utilizador WHERE id = %s
		"""

	values = (
		user_id,
	)
	#cur.execute("Begin transaction")
	#cur.execute("LOCk table utilizador in EXCLUSIVE MODE")
	cur.execute(statement, values)

	admin = cur.fetchall()
	#cur.execute("commit")

	conn.close()

	if not admin:
		return False

	return admin[0][0]

def sendMessageToAllBidders(ean):
	conn = db_connection()
	cur = conn.cursor()

	statement = """
		INSERT INTO utilizador_mensagem (utilizador_id, mensagem_id)
		SELECT DISTINCT(licitacao.utilizador_id), (SELECT MAX(id) FROM mensagem)
		FROM licitacao
		WHERE licitacao.leilao_ean = %s
		"""

	values = (
		ean,
		)
	#cur.execute("Begin transaction")
	cur.execute(statement,values)
	cur.execute("commit")
	conn.close()

def createMessageToAllBidders(ean):
	conn = db_connection()
	cur = conn.cursor()

	#cur.execute("Begin transaction")
	#cur.execute("LOCk table leilao in EXCLUSIVE MODE")
	cur.execute("SELECT utilizador_id FROM leilao WHERE ean = %s", (str(ean),))
	remetente = cur.fetchall()
	#cur.execute("commit")
	if not remetente:
		return 1

	statement = """
		INSERT INTO mensagem(message, leilao_ean, utilizador_id)
		VALUES ('Leilao terminado por Admin. Peco desculpa pelo incomodo', %s, %s)
		"""

	values = (
		ean,
		remetente[0][0]
		)

	#cur.execute("Begin transaction")
	cur.execute(statement, values)
	cur.execute("commit")

	sendMessageToAllBidders(ean)
	conn.close()


def sendMessageToLosers():
	conn = db_connection()
	cur = conn.cursor()

	#cur.execute("Begin transaction")
	cur.execute("""
		INSERT INTO utilizador_mensagem (utilizador_id, mensagem_id)
		SELECT DISTINCT(licitacao.utilizador_id),
		(SELECT max(mensagem.id) FROM mensagem, leilao WHERE CURRENT_TIMESTAMP >= leilao.endingdate AND leilao.state = TRUE and leilao.ean = mensagem.leilao_ean)
		FROM licitacao, leilao
		WHERE licitacao.leilao_ean = leilao.ean AND CURRENT_TIMESTAMP >= leilao.endingdate AND leilao.state = TRUE
		"""
		)

	cur.execute("commit")

	conn.close()

def messageToLosers():
	conn = db_connection()
	cur = conn.cursor()

	#cur.execute("Begin transaction")
	cur.execute("""
		INSERT INTO mensagem(message, leilao_ean, utilizador_id)
		SELECT 'Leilao terminado', leilao.ean, leilao.utilizador_id
		FROM leilao
		WHERE CURRENT_TIMESTAMP >= leilao.endingdate AND leilao.state = TRUE
		"""
		)

	cur.execute("commit")
	sendMessageToLosers()

	conn.close()


def sendMessageToWinner():
	conn = db_connection()
	cur = conn.cursor()

	#cur.execute("Begin transaction")
	cur.execute("""
		INSERT INTO utilizador_mensagem (utilizador_id, mensagem_id)
		SELECT licitacao.utilizador_id, (SELECT max(mensagem.id) FROM mensagem, leilao WHERE CURRENT_TIMESTAMP >= leilao.endingdate AND leilao.state = TRUE and leilao.ean = mensagem.leilao_ean)
		FROM licitacao, leilao
		WHERE leilao.ean = licitacao.leilao_ean AND licitacao.bid = leilao.price AND CURRENT_TIMESTAMP >= leilao.endingdate AND leilao.state = TRUE
		"""
		)
	cur.execute("commit")
	conn.close()

def messageToWinner():
	conn = db_connection()
	cur = conn.cursor()

	#cur.execute("Begin transaction")
	cur.execute("""
		INSERT INTO mensagem(message, leilao_ean, utilizador_id)
		SELECT 'Voce ganhou o leilao', ean, leilao.utilizador_id
		FROM leilao
		WHERE CURRENT_TIMESTAMP >= leilao.endingdate AND leilao.state = TRUE
		"""
		)
	cur.execute("commit")
	sendMessageToWinner()

	conn.close()


def countToEndAuctions():
	conn = db_connection()
	cur = conn.cursor()

	#cur.execute("Begin transaction")
	#cur.execute("LOCk table leilao in EXCLUSIVE MODE")
	cur.execute("""
				SELECT COUNT(*) FROM leilao
				WHERE CURRENT_TIMESTAMP >= endingdate
				AND state = TRUE
				""")

	number = cur.fetchall()
	#cur.execute("commit")

	conn.close()
	return number[0][0]


def emailExist(email):
	conn = db_connection()
	cur = conn.cursor()

	statement = """
		SELECT email FROM utilizador WHERE email = %s
		"""

	values = (
		email,
	)
	#cur.execute("Begin transaction")
	#cur.execute("LOCk table utilizador in EXCLUSIVE MODE")
	cur.execute(statement, values)

	result = cur.fetchall()
	#cur.execute("commit")

	conn.close()

	if not result:
		return False
	else:
		return True



##########################################################
## USER
##########################################################

# ---------------- PRINT ALL USERS ---------------- #

@app.route("/dbproj/user/", methods=['GET'], strict_slashes=True)
def get_all_users():
	logger.info("###               GET /dbproj/user/             ###");   

	conn = db_connection()
	cur = conn.cursor()

	#cur.execute("Begin transaction")
	#cur.execute("LOCk table utilizador in EXCLUSIVE MODE")
	cur.execute("SELECT * from utilizador")
	rows = cur.fetchall()
	#cur.execute("commit")

	payload = []
	logger.debug("---- USERS  ----")
	for row in rows:
		logger.debug(row)
		content = {
		 'status': row[0],
		 'id': row[1],
		 'name': row[2],
		 'surname': row[3],
		 'email': row[4],
		 'address': row[5],
		 'city': row[6],
		 'gender': row[7],
		 'password': row[8],
		 'admin': row[9]
		}
		payload.append(content) # appending to the payload to be returned

	conn.close()
	return jsonify(payload)

# ---------------- REGISTER ---------------- #

@app.route("/dbproj/user/", methods=['POST'])
def register_user():
	logger.info("###              POST /dbproj/user/              ###");   

	conn = db_connection()
	cur = conn.cursor()

	payload = request.get_json()

	logger.info("---- New User  ----")
	logger.debug(f'payload: {payload}')

	if emailExist(payload["email"]):
		result = 'Email Already In Used!'
		conn.close()
		return jsonify(result)

	# parameterized queries, good for security and performance
	statement = """
		INSERT INTO utilizador (name, surname, email, address, city, gender, password, admin)
		VALUES (%s, %s, %s, %s, %s, %s, %s, %s);
		"""

	values = (
		payload["name"],
		payload["surname"],
		payload["email"],
		payload["address"],
		payload["city"],
		payload["gender"],
		generate_password_hash(payload["password"]),
		payload["admin"],
	)
	try:
		#cur.execute("Begin transaction")
		cur.execute(statement, values)
		cur.execute("commit")
		if not payload["admin"]:
			result = 'New User Inserted!'
		else:
			result = 'New Admin Inserted!'
	except (Exception, psycopg2.DatabaseError) as error:
		logger.error(error)
		result = 'Fail to Insert New User!'
	finally:
		if conn is not None:
			conn.close()

	return jsonify(result)

# ---------------- LOGIN ---------------- #

@app.route("/dbproj/user/", methods=['PUT'])
def login_user():
	logger.info("###              PUT /dbproj/user/              ###");   

	conn = db_connection()

	payload = request.get_json()

	logger.info("---- LOGIN  ----")
	logger.info(f'payload: {payload}')

	token = verifyPasswordAndReturnToken(payload["password"], payload["email"])
	logger.info(type(token))

	if token == -1:
		result = f'Login Failed!'
		conn.close()
		return jsonify(result)
	elif token == -2:
		result = f'User not registered!'
		conn.close()
		return jsonify(result)
	else:
		try:
			answer = f'Login with Success!'
			result = {
				'resposta': answer,
				'token': token
			}
		except (Exception, psycopg2.DatabaseError) as error:
			logger.error(error)
			result = {'resposta': 'Failed!'}
		finally:
			if conn is not None:
				conn.close()
		return jsonify(result)
		
	conn.close()
	return jsonify(result)


# ---------------- SEARCH USER ACTIVITY ---------------- #

@app.route("/dbproj/user/activity/", methods=['GET'])
@token_required
def search_user_activity(current_user):
	logger.info("###              GET /dbproj/user/<user_id>              ###");   

	conn = db_connection()
	cur = conn.cursor()
	#cur.execute("Begin transaction")
	#cur.execute("LOCk table leilao, descricao in EXCLUSIVE MODE")

	cur.execute("""
		SELECT leilao.ean, leilao.title, leilao.endingdate, descricao.description, leilao.utilizador_id
		FROM leilao JOIN descricao ON leilao.ean = descricao.leilao_ean
		WHERE leilao.utilizador_id = %s AND descricao.currentdescription = true
		UNION
		SELECT leilao.ean, leilao.title, leilao.endingdate, descricao.description, leilao.utilizador_id
		FROM leilao JOIN descricao ON leilao.ean = descricao.leilao_ean
		JOIN licitacao ON leilao.ean = licitacao.leilao_ean
		WHERE licitacao.utilizador_id = %s AND descricao.currentdescription = true""", (current_user, current_user,) )

	rows = cur.fetchall()
	#cur.execute("commit")
	payload = []
	if not rows:
		content = 'No activity yet!'
		payload.append(content)
	else:
		for row in rows:
			content = {
				'ean': int(row[0]),
				'title': row[1],
				'endingdate': row[2],
				'descricao': row[3],
				'criador': row[4]
			}
			payload.append(content) # appending to the payload to be returned

	conn.close()
	return jsonify(payload)

# ---------------- BID ---------------- #

@app.route("/dbproj/licitar/<leilaoId>/<licitacao>", methods=['POST'])
@token_required
def licitar(current_user, leilaoId, licitacao):
	logger.info("###             POST /dbproj/licitar/<leilaoId>/<licitacao>              ###");   


	conn = db_connection()
	cur = conn.cursor()

	if isAdmin(current_user) == True:
		result = "Error: Admins cant bid!"
		conn.close()
		return jsonify(result)


	owner = isOwner(current_user, leilaoId)
	if owner == 0:
		result = "Error: You Own this Auction!"
		conn.close()
		return jsonify(result)
	if owner == -2:
		result = "Error: Auction does not exist!"
		conn.close()
		return jsonify(result)

	if not isFinished(leilaoId):
		result = "Error: Auction Ended"
		conn.close()
		return jsonify(result)

	statement = """
		INSERT INTO licitacao (bid, date, valid, utilizador_id, leilao_ean)
		SELECT %s, CURRENT_TIMESTAMP, TRUE, %s, %s FROM leilao, utilizador
		WHERE %s = leilao.ean AND %s > leilao.price AND leilao.state = true AND utilizador.status = true AND utilizador.id = %s
		"""

	values = (
		licitacao,
		current_user,
		leilaoId,
		leilaoId,
		licitacao,
		current_user
	)
	
	try:
		#cur.execute("Begin transaction")
		#cur.execute("LOCk table licitacao in EXCLUSIVE MODE")
		cur.execute("""SELECT COUNT(*) FROM licitacao""")
		antes = cur.fetchall()
		#cur.execute("commit")
		#cur.execute("Begin transaction")
		cur.execute(statement, values)
		cur.execute("commit")
		#cur.execute("Begin transaction")
		#cur.execute("LOCk table licitacao in EXCLUSIVE MODE")
		cur.execute("""SELECT COUNT(*) FROM licitacao""")
		depois = cur.fetchall()
		#cur.execute("Commit")
		if (depois[0][0]-antes[0][0] > 0):
			result = 'Bid Inserted!'
			update_price(leilaoId, licitacao)
		else:
			result = 'Bid Too Low!'
		return jsonify(result)
	except (Exception, psycopg2.DatabaseError) as error:
		logger.error(error)
		result = 'Fail to insert  bid!'
		return jsonify(result)
	finally:
		if conn is not None:
			conn.close()

	return jsonify(result)


##########################################################
## MESSAGE
##########################################################

# ---------------- NEW MESSAGE ---------------- #

def send_mailbox(leilaoId, utilizador_id):

	conn = db_connection()
	cur = conn.cursor()

	# parameterized queries, good for security and performance
	statement ="""
				INSERT INTO utilizador_mensagem(utilizador_id, mensagem_id)
				SELECT DISTINCT(mensagem.utilizador_id), (SELECT MAX(id) FROM mensagem)
				FROM mensagem
				WHERE mensagem.leilao_ean = %s AND mensagem.utilizador_id <> %s
				"""

	values = (
		leilaoId,
		utilizador_id
	)

	try:
		#cur.execute("Begin transaction")
		cur.execute(statement, values)
		cur.execute("commit")
	except (Exception, psycopg2.DatabaseError) as error:
		logger.error(error)
	finally:
		if conn is not None:
			conn.close()


@app.route("/dbproj/message/<leilaoId>", methods=['POST'])
@token_required
def add_message(current_user, leilaoId):
	logger.info("###              POST //dbproj/leilao/<leilaoId>              ###");   
	payload = request.get_json()

	conn = db_connection()
	cur = conn.cursor()

	logger.info("---- New Message  ----")
	logger.info(f'payload: {payload}')

	# parameterized queries, good for security and performance
	statement ="""
				INSERT INTO mensagem(message, leilao_ean, utilizador_id)
				VALUES (%s, %s, %s)
				"""

	values = (
		payload["message"],
		leilaoId,
		current_user
	)

	try:
		#cur.execute("Begin transaction")
		cur.execute(statement, values)
		cur.execute("commit")
		send_mailbox(leilaoId, current_user)
		result = 'Message Sent!'
	except (Exception, psycopg2.DatabaseError) as error:
		logger.error(error)
		result = 'Failed to send message!'
	finally:
		if conn is not None:
			conn.close()
	return jsonify(result)


# ---------------- MAILBOX ---------------- #

@app.route("/dbproj/mailbox/", methods=['GET'])
@token_required
def mailbox(current_user):
	logger.info("###              GET /dbproj/mailbox/            ###");

	conn = db_connection()
	cur = conn.cursor()

	#cur.execute("Begin transaction")
	#cur.execute("LOCk table mensagem, utilizador_mensagem in EXCLUSIVE MODE")
	cur.execute("""
		SELECT mensagem.leilao_ean, mensagem.utilizador_id, mensagem.message
		FROM mensagem, utilizador_mensagem
		WHERE utilizador_mensagem.mensagem_id = mensagem.id AND utilizador_mensagem.utilizador_id = %s
		""", (current_user,) )

	rows = cur.fetchall()
	#cur.execute("Commit")
	payload = []
	if not rows:
		content = 'Mailbox Empty!'
		payload.append(content)
	else:
		for row in rows:
			content = {
				'ean': int(row[0]),
				'id_remetente': row[1],
				'message': row[2]
			}
			payload.append(content) # appending to the payload to be returned

	conn.close()
	return jsonify(payload)


# ---------------- FEED ---------------- #

@app.route("/dbproj/feed/<ean>", methods=['GET'])
@token_required
def feed(current_user, ean):
	logger.info("###              GET /dbproj/feed/<ean>              ###");   

	conn = db_connection()
	cur = conn.cursor()

	#cur.execute("Begin transaction")
	#cur.execute("LOCk table mensagem in EXCLUSIVE MODE")
	cur.execute("""
		SELECT mensagem.utilizador_id, mensagem.message
		FROM mensagem
		WHERE mensagem.leilao_ean = %s;
		""", (ean,) )

	rows = cur.fetchall()
	#cur.execute("Commit")
	payload = []
	if not rows:
		content = 'Auction does not exist!'
		payload.append(content)
	else:
		for row in rows:
			content = {
				'utilizador_id': int(row[0]),
				'message': row[1]
			}
			payload.append(content) # appending to the payload to be returned

	conn.close()
	return jsonify(payload)



##########################################################
## AUCTION
##########################################################

# ---------------- PRINT ALL ACTIVE AUCTION (id - description) ---------------- #

@app.route("/dbproj/leiloes/", methods=['GET'], strict_slashes=True)
@token_required
def get_all_leiloes(current_user):
	logger.info("###              GET /dbproj/leiloes/              ###");   

	conn = db_connection()
	cur = conn.cursor()

	#cur.execute("Begin transaction")
	#cur.execute("LOCk table leilao, descricao in EXCLUSIVE MODE")
	cur.execute("SELECT leilao.ean, descricao.description FROM leilao, descricao WHERE leilao.state = true AND leilao.ean = descricao.leilao_ean AND descricao.currentdescription = true")
	rows = cur.fetchall()
	#cur.execute("Commit")

	payload = []
	logger.debug("---- Leiloes  ----")
	for row in rows:
		logger.debug(row)
		content = {
		 'ean': int(row[0]),
		 'descricao': row[1]
		}
		payload.append(content) # appending to the payload to be returned

	conn.close()
	return jsonify(payload)

# ---------------- NEW AUCTION ---------------- #

@app.route("/dbproj/leilao/", methods=['POST'])
@token_required
def add_leilao(current_user):
	logger.info("###              POST /dbproj/leilao/              ###");   

	conn = db_connection()
	cur = conn.cursor()

	payload = request.get_json()
	logger.info(current_user)
	logger.info(type(current_user))

	if isAdmin(current_user) == True:
		result = "Admins cant create auctions!"
		conn.close()
		return jsonify(result)

	logger.info("---- new leilao  ----")
	logger.debug(f'payload: {payload}')

	# parameterized queries, good for security and performance
	statement = """
		INSERT INTO leilao (ean, title, price, endingdate, state, utilizador_id)
		VALUES (%s, %s, %s, %s, TRUE, %s);
		"""

	values = (
		payload["ean"],
		payload["title"],
		payload["price"],
		payload["endingdate"],
		current_user
	)
	

	try:
		#cur.execute("Begin transaction")
		cur.execute(statement, values)
		cur.execute("commit")
		result = 'Inserted!'
		add_description(payload["ean"], payload["description"])
	except (Exception, psycopg2.DatabaseError) as error:
		logger.error(error)
		result = 'Failed!'
	finally:
		if conn is not None:
			conn.close()

	return jsonify(result)


# ---------------- SEARCH AUCTION ---------------- #

@app.route("/dbproj/leiloes/<keyword>", methods=['GET'])
@token_required
def search_auction(current_user, keyword):
	logger.info("###              GET /dbproj/leiloes/<keyword>              ###");   

	conn = db_connection()
	cur = conn.cursor()

	#cur.execute("Begin transaction")
	#cur.execute("LOCk table leilao, descricao in EXCLUSIVE MODE")
	if keyword.isdigit():
		cur.execute("SELECT ean, description FROM leilao, descricao WHERE leilao.ean = descricao.leilao_ean AND ean = %s AND currentdescription = TRUE", (keyword,) )
	else:
		search_term = keyword
		like_pattern = '%{}%'.format(search_term)
		cur.execute("SELECT ean, description FROM leilao, descricao WHERE  leilao.ean = descricao.leilao_ean AND LOWER(description) LIKE LOWER(%s) AND currentdescription = TRUE", (like_pattern,))

	rows = cur.fetchall()
	#cur.execute("Commit")
	payload = []
	if not rows:
		content = 'Auction Not Found!'
		payload.append(content)
	else:
		for row in rows:
			content = {
				'ean': int(row[0]),
				'descricao': row[1]
			}
			payload.append(content) # appending to the payload to be returned

	conn.close()
	return jsonify(payload)

# ---------------- SEARCH AUCTION BY ID ---------------- #

@app.route("/dbproj/leilao/<leilaoId>", methods=['GET'])
@token_required
def search_auction_full_Info(current_user, leilaoId):
	logger.info("###            GET /dbproj/leilao/<leilaoId              ###");   

	conn = db_connection()
	cur = conn.cursor()

	#cur.execute("Begin transaction")
	#cur.execute("LOCk table leilao, descricao in EXCLUSIVE MODE")
	cur.execute("SELECT * FROM leilao, descricao WHERE ean = %s AND descricao.leilao_ean = %s AND descricao.currentdescription = true", (leilaoId, leilaoId) )

	rows = cur.fetchall()
	#cur.execute("Commit")


	payload = []
	logger.debug("---- leilao  ----")
	for row in rows:
		logger.debug(row)
		content = {
		'ean': int(row[0]),
		'title': row[1],
		'price': row[2],
		'endingdate': row[3],
		'state': row[4],
		'utilizador_id': int(row[5]),
		'description': row[7]
		}
		payload.append(content) # appending to the payload to be returned

	conn.close()
	return jsonify(payload)


# ---------------- EDIT AUCTION DESCRIPTION (actually creating new description) ---------------- #

@app.route("/dbproj/leilao/<leilaoId>", methods=['POST'])
@token_required
def edit_auction_description(current_user, leilaoId):
	logger.info("###              POST //dbproj/leilao/<leilaoId>              ###");

	conn = db_connection()
	cur = conn.cursor()

	payload = request.get_json()

	owner = isOwner(current_user, leilaoId)

	if owner == -1:
		result = "You don't own this auction!"
		conn.close()
		return jsonify(result)
	elif owner == -2:
		result = "Auction not found!"
		conn.close()
		return jsonify(result)

	logger.info("---- update department  ----")
	logger.info(f'payload: {payload}')

	# parameterized queries, good for security and performance
	statement ="""
				INSERT INTO descricao(description, currentdescription, modifieddate, leilao_ean)
				VALUES (%s, true, CURRENT_TIMESTAMP, %s)
				"""

	values = (
		payload["description"],
		leilaoId
	)

	try:
		#cur.execute("Begin transaction")
		cur.execute(statement, values)
		cur.execute("commit")
		result = 'Auction description edited with success'
	except (Exception, psycopg2.DatabaseError) as error:
		logger.error(error)
		result = 'Failed to edit Auction!'
	finally:
		if conn is not None:
			conn.close()
	return jsonify(result)

# ---------------- END AUCTION BY TIME ---------------- #

@app.route("/dbproj/leilao/time_end", methods=['PUT'])
def endAuctionByTime():
	logger.info("###              PUT /dbproj/user/              ###");   

	conn = db_connection()
	cur = conn.cursor()

	payload = request.get_json()

	logger.info("---- END AUCTION  ----")
	logger.info(f'payload: {payload}')

	number = countToEndAuctions()
	if number == 0:
		result = "No Auction to Expire!"
		return jsonify(result)

	messageToLosers()
	messageToWinner()

	# parameterized queries, good for security and performance
	#cur.execute("Begin transaction")
	cur.execute("""
				UPDATE leilao
				SET state = FALSE
				WHERE CURRENT_TIMESTAMP >= endingdate
				AND state = TRUE
				""")
	cur.execute("commit")

	conn.close()
	return jsonify('Ended: ' + str(number) + ' auction(s).')


##########################################################
## ADMIN TOOLS
##########################################################

# ---------------- FORCE END AUCTION ---------------- #

@app.route("/dbproj/leilao/force_end/", methods=['PUT'])
@token_required
def ForceEndAuction(current_user):
	logger.info("###              PUT /dbproj/user/              ###");   

	conn = db_connection()
	cur = conn.cursor()

	payload = request.get_json()

	logger.info("---- END AUCTION  ----")
	logger.info(f'payload: {payload}')


	if isAdmin(current_user) == True:

		if createMessageToAllBidders(payload["ean"]) == 1:
			return jsonify('Auction does not exist!')

		# parameterized queries, good for security and performance
		statement ="""
					UPDATE leilao 
					SET state = FALSE
					WHERE ean = %s
					"""

		values = (
			payload["ean"],
			)

		try:
			#cur.execute("Begin transaction")
			cur.execute(statement, values)
			cur.execute("commit")
			result = 'Forced auction deactivation!'
		except (Exception, psycopg2.DatabaseError) as error:
			logger.error(error)
			result = 'Failed to end auction!'
		finally:
			if conn is not None:
				conn.close()
		return jsonify(result)
	else:
		result = 'You are not admin!'
	conn.close()
	return jsonify(result)

# ---------------- STATS ---------------- #

@app.route("/dbproj/stats/", methods=['GET'])
@token_required
def stats(current_user):
	logger.info("###              PUT /dbproj/stats/             ###");   

	conn = db_connection()
	cur = conn.cursor()

	if isAdmin(current_user) == True:
		cur.callproc('estatistica_leiloes_criados', ())
		top0 = cur.fetchall()

		cur.callproc('estatistica_leiloes_winners', ())
		top1 = cur.fetchall()

		cur.callproc('estatistica_ten_days', ())
		top2 = cur.fetchall()[0][0]

		conn.close()
		return jsonify(("TOP 10 USERS WITH MORE AUCTIONS:", top0, "TOP 10 WINNERS:", top1, "TOTAL AUCTIONS FROM LAST 10 DAYS:", top2))
	else:
		return jsonify('You are not admin!')


# ---------------- BAN ---------------- #

@app.route("/dbproj/ban/<user_id>", methods=['PUT'])
@token_required
def ban(current_user, user_id):
	logger.info("###              PUT /dbproj/ban/<user>              ###");   

	conn = db_connection()
	cur = conn.cursor()

	payload = request.get_json()

	logger.info("----   ----")
	logger.info(f'payload: {payload}')

	if isAdmin(current_user) == False:
		result = "You are not admin!"
		conn.close()
		return jsonify(result)


	# parameterized queries, good for security and performance
	statement = """
				UPDATE utilizador
				SET status = FALSE
				WHERE id = %s and admin = FALSE
				"""
	values = (
		user_id,
		)

	try:
		cur.execute(statement, values)
		result = 'User '+user_id+' banned!'
		cur.callproc('kill_user', (user_id))
		#cur.execute("Begin transaction")
		#cur.execute("LOCk table leilao in EXCLUSIVE MODE")
		cur.execute("SELECT ean FROM leilao WHERE utilizador_id = %s", (user_id))
		leiloes = cur.fetchall()
		#cur.execute("Commit")
		#cur.execute("Begin transaction")
		for leilao in leiloes:
			createMessageToAllBidders(leilao[0])
			cur.execute("UPDATE leilao SET state = FALSE WHERE ean = %s", (str(leilao[0])))
			cur.execute("commit")
	except (Exception, psycopg2.DatabaseError) as error:
		logger.error(error)
		result = 'Failed to ban user!'
	finally:
		if conn is not None:
			conn.close()
			return jsonify(result)
	conn.close()
	return jsonify(result)


##########################################################
## DATABASE ACCESS
##########################################################

def db_connection():
	db = psycopg2.connect(user = "grupo",
							password = "grupo",
							host = "db",
							port = "5432",
							database = "projeto")
	return db


##########################################################
## MAIN
##########################################################
if __name__ == "__main__":

	# Set up the logging
	logging.basicConfig(filename="logs/log_file.log")
	logger = logging.getLogger('logger')
	logger.setLevel(logging.DEBUG)
	ch = logging.StreamHandler()
	ch.setLevel(logging.DEBUG)

	# create formatter
	formatter = logging.Formatter('%(asctime)s [%(levelname)s]:  %(message)s',
							  '%H:%M:%S')
							  # "%Y-%m-%d %H:%M:%S") # not using DATE to simplify
	ch.setFormatter(formatter)
	logger.addHandler(ch)


	time.sleep(1) # just to let the DB start before this print :-)


	logger.info("\n---------------------------------------------------------------\n" + 
				  "API v1.0 online: http://localhost:8080/dbproj/\n\n")

	f = open("chave_secreta.txt", "r")
	app.config['SECRET_KEY'] = f.readline()
	f.close()
	#logger.info(app.config['SECRET_KEY'])

	app.run(host="0.0.0.0", debug=True, threaded=True)