from reportlab.lib.pagesizes import letter
from reportlab.pdfgen import canvas
from reportlab.lib.units import inch
from PIL import Image
import io
import os
import tempfile
import shutil

# User modifiable parameters
LEFT_MARGIN = 3/8  # in inches
RIGHT_MARGIN = 3/8 #3/8
TOP_MARGIN = 0.5
BOTTOM_MARGIN = 0.5 #1/2
CIRCLE_DIAMETER = 1.0  # diameter of each circle in inches
NUM_COLUMNS = 7
NUM_ROWS = 9
OUTPUT_FILENAME = "1-Inch-Stickers.pdf"
LOGO_PATH = "OpenSpoolLogo1Inch.png"  # Path to your 1-inch logo PNG file

def prepare_logo(logo_path, desired_size_inches):
    """
    Prepare the logo image for insertion into PDF.
    Creates a temporary copy of the logo if modifications are needed.
    
    Args:
        logo_path (str): Path to the logo PNG file
        desired_size_inches (float): Desired size in inches
        
    Returns:
        str: Path to the temporary logo file
    """
    # Create a temporary file with the same extension as the original
    temp_dir = tempfile.gettempdir()
    temp_logo_path = os.path.join(temp_dir, 'temp_logo.png')
    
    # Make a copy of the original logo
    shutil.copy2(logo_path, temp_logo_path)
    
    # Open and resize the image if necessary
    img = Image.open(temp_logo_path)
    
    # Convert desired inches to pixels (assuming 72 DPI for PDF)
    desired_size_points = int(desired_size_inches * 72)
    
    needs_modification = False
    
    # Resize image if it's not the right size
    if img.size != (desired_size_points, desired_size_points):
        img = img.resize((desired_size_points, desired_size_points), Image.Resampling.LANCZOS)
        needs_modification = True
    
    # Convert to RGB if necessary (PDF doesn't support RGBA)
    if img.mode == 'RGBA':
        # Create a white background
        background = Image.new('RGB', img.size, (255, 255, 255))
        # Paste the image using alpha channel as mask
        background.paste(img, mask=img.split()[3])
        img = background
        needs_modification = True
    elif img.mode != 'RGB':
        img = img.convert('RGB')
        needs_modification = True
    
    # Save modifications to the temporary file if needed
    if needs_modification:
        img.save(temp_logo_path)
    
    return temp_logo_path

def create_circle_pdf(filename, 
                     logo_path=LOGO_PATH,
                     left_margin=LEFT_MARGIN*inch, 
                     right_margin=RIGHT_MARGIN*inch,
                     top_margin=TOP_MARGIN*inch, 
                     bottom_margin=BOTTOM_MARGIN*inch,
                     circle_diameter=CIRCLE_DIAMETER*inch,
                     num_columns=NUM_COLUMNS,
                     num_rows=NUM_ROWS,
                     oversize_factor=1.05):
    """
    Generate a PDF with evenly spaced circles and logo overlays arranged in a grid.
    
    Args:
        filename (str): Output PDF filename
        logo_path (str): Path to the logo PNG file
        left_margin (float): Left margin in points
        right_margin (float): Right margin in points
        top_margin (float): Top margin in points
        bottom_margin (float): Bottom margin in points
        circle_diameter (float): Diameter of each circle in points
        num_columns (int): Number of columns in the grid
        num_rows (int): Number of rows in the grid
        oversize_factor (float): Make the image slightly larger to account for tolerances
    """
    try:

        circle_diameter *= oversize_factor

        # Prepare the logo
        temp_logo_path = prepare_logo(logo_path, CIRCLE_DIAMETER)
        
        # Set up the canvas
        c = canvas.Canvas(filename, pagesize=letter)
        width, height = letter
        circle_radius = circle_diameter / 2
        num_circles = num_columns * num_rows

        # Calculate available space
        available_width = width - left_margin - right_margin
        available_height = height - top_margin - bottom_margin

        # Calculate spacing between circles
        total_circle_width = num_columns * circle_diameter
        total_gaps_width = available_width - total_circle_width
        x_spacing = total_gaps_width / (num_columns - 1)

        total_circle_height = num_rows * circle_diameter
        total_gaps_height = available_height - total_circle_height
        y_spacing = total_gaps_height / (num_rows - 1)

        # Draw circles and logos
        for i in range(num_rows):
            for j in range(num_columns):
                if i * num_columns + j < num_circles:
                    # Calculate center positions
                    x = left_margin + (j * (circle_diameter + x_spacing))
                    y = height - (top_margin + (i * (circle_diameter + y_spacing)) + circle_diameter)
                    
                    # Draw circle
                    c.circle(x + circle_radius, y + circle_radius, circle_radius, stroke=1, fill=0)
                    
                    # Draw logo
                    c.drawImage(temp_logo_path, x, y, width=circle_diameter, height=circle_diameter, mask='auto')

        # Save the PDF
        c.save()
    
    finally:
        # Clean up the temporary file
        if 'temp_logo_path' in locals() and os.path.exists(temp_logo_path):
            os.remove(temp_logo_path)

if __name__ == "__main__":
    # Generate the PDF file with default parameters
    create_circle_pdf(OUTPUT_FILENAME)