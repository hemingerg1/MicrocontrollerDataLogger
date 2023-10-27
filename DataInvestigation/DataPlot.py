import os
import tkinter as tk
from tkinter import filedialog
import pandas as pd
import plotly.graph_objects as go

# Function to open and plot the selected CSV file
def open_csv_file():
    # Open file dialog to select a CSV file
    file_path = filedialog.askopenfilename(initialdir=".Data", title="Select CSV file", filetypes=(("CSV files", "*.csv"), ("All files", "*.*")))
    filename = os.path.basename(file_path)

    # Check if a file was selected
    if file_path:
        # Read the CSV file into a pandas DataFrame
        df = pd.read_csv(file_path, names=['Time', 'Sensor0Time', 'Sensor0Data', 'Sensor1Time', 'Sensor1Data']) #index_col=0,
        df = df.drop(columns=['Time'])
        startTime = df.Sensor0Time.min()
        df['Sensor0Time'] = df['Sensor0Time'] - startTime
        df['Sensor1Time'] = df['Sensor1Time'] - startTime
        #print(df)
        
        # Create a subplot with a single trace
        fig = go.Figure()
        fig.add_trace(go.Scatter(x=df['Sensor0Time'], y=df['Sensor0Data'], mode='lines+markers', name='Sensor0', line_shape='linear', hoverinfo='none'))
        fig.add_trace(go.Scatter(x=df['Sensor1Time'], y=df['Sensor1Data'], mode='lines+markers', name='Sensor1', line_shape='linear', hoverinfo='none'))
        
        # Configure the layout
        fig.update_layout(title=dict(text=filename, xanchor='center', x=0.5),
                          xaxis_title='Time',
                          yaxis_title='Pressure (psi)',
                          font=dict(color='#929292', size=12),
                          modebar=dict(remove=['lasso', 'select2d', 'autoscale']),
                          paper_bgcolor='#212121',
                          plot_bgcolor='#464646',
                          xaxis_rangeslider_visible=True,
                          hovermode='x unified',
                          dragmode='pan'
                          )
        
        fig.update_xaxes(gridcolor='#717171', griddash='dash', spikemode='across', spikesnap='data', spikecolor='#929292', spikethickness=1, spikedash='solid')
        fig.update_yaxes(gridcolor='#717171', griddash='dash', spikemode='across', spikesnap='cursor', spikecolor='#929292', spikethickness=1, spikedash='solid')
        
        fig.show()

# Create the tkinter app
app = tk.Tk()
app.title("CSV File Viewer")
app.geometry("300x75")

# Create a frame to hold the button
frame = tk.Frame(app)
frame.pack(expand=True)

# Create a button to open the CSV file
open_button = tk.Button(frame, text="Open CSV File", command=open_csv_file, padx=5, pady=5)
open_button.pack(anchor="center")

# Run the tkinter main loop
app.mainloop()