import logging
from telegram import Update
from telegram.ext import ApplicationBuilder, ContextTypes, CommandHandler

import Server.db

logging.basicConfig(
    format='%(asctime)s - %(name)s - %(levelname)s - %(message)s',
    level=logging.INFO
)

async def ajuda(update: Update, context: ContextTypes.DEFAULT_TYPE):
    await context.bot.send_message(chat_id=update.effective_chat.id, text='''
    Comandes: \n
    \t - /start - Missatge de benvinguda del bot
    \t - /ajuda - Comandes disponibles
    \t - /metrics - Dades meteorològiques de la vivenda
    ''')

async def metrics(update: Update, context: ContextTypes.DEFAULT_TYPE):
    
    data_int = Server.db.get_last_temp_humidity(1)
    data_ext = Server.db.get_last_temp_humidity(2)
    
    await context.bot.send_message(chat_id=update.effective_chat.id, text='''- Interior 🏠\n\tTemperature Int: 🌡️ {} ºC \n\tHumidity Int : 💧 {} %\n\n- Exterior ⛺\n\tTemperature Ext: 🌡️ {} ºC \n\tHumidity Ext : 💧 {} %
    '''.format(data_int[0][0],data_int[0][1],data_ext[0][0],data_ext[0][1]))

async def start(update: Update, context: ContextTypes.DEFAULT_TYPE):
    logging.log(logging.INFO,'Start from : {} '.format(update.message.chat.id))
    await context.bot.send_message(chat_id=update.effective_chat.id, text="🌞❄🏠\nAquest \"bot\" permet la consulta de les dades meteorològiques de la teva vivenda. Escriu /ajuda per saber quines comandes pots utilitzar.")

if __name__ == '__main__':
    
    Server.db.connect()    

    application = ApplicationBuilder().token('<TOKEN>').build()
    
    start_handler = CommandHandler('start', start)
    ajuda_handler = CommandHandler('ajuda', ajuda)
    metrics_handler = CommandHandler('metrics', metrics)

    application.add_handler(start_handler)
    application.add_handler(ajuda_handler)
    application.add_handler(metrics_handler)
    
    application.run_polling()
