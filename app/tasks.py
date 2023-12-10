from celery import Celery

app = Celery('tasks', backend='rpc://', broker='pyamqp://guest@localhost//')
app.conf.CELERY_RESULT_BACKEND = 'db+sqlite://results/sqlite'

@app.task
def add(x, y):
    return x + y