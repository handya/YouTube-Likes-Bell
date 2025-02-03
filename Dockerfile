# Use official lightweight Python image
FROM python:3.10-slim

# Set working directory
WORKDIR /app

# Copy script into container
COPY youtube_likes.py .

# Install required packages
RUN pip install requests

# Run script
CMD ["python", "youtube_likes.py"]
