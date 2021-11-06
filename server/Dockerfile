FROM python:3.9.0-alpine3.12

EXPOSE 80

RUN apk update
RUN apk upgrade

RUN pip install flask

CMD [ "python", "-u", "/mnt/paperplane/server.py" ]
