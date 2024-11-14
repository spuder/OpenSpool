from reportlab.lib.pagesizes import letter
from reportlab.pdfgen import canvas
from reportlab.lib.units import inch

# User modifiable parameters
LEFT_MARGIN = 3/8  # in inches
RIGHT_MARGIN = 3/8 #3/8
TOP_MARGIN = 0.5
BOTTOM_MARGIN = 0.5 #1/2
CIRCLE_DIAMETER = 1.0  # diameter of each circle in inches
NUM_COLUMNS = 7
NUM_ROWS = 9
OUTPUT_FILENAME = "1-Inch-Template.pdf"

def create_circle_pdf(filename, 
                     left_margin=LEFT_MARGIN*inch, 
                     right_margin=RIGHT_MARGIN*inch,
                     top_margin=TOP_MARGIN*inch, 
                     bottom_margin=BOTTOM_MARGIN*inch,
                     circle_diameter=CIRCLE_DIAMETER*inch,
                     num_columns=NUM_COLUMNS,
                     num_rows=NUM_ROWS):
    """
    Generate a PDF with evenly spaced circles arranged in a grid.
    
    Args:
        filename (str): Output PDF filename
        left_margin (float): Left margin in points
        right_margin (float): Right margin in points
        top_margin (float): Top margin in points
        bottom_margin (float): Bottom margin in points
        circle_diameter (float): Diameter of each circle in points
        num_columns (int): Number of columns in the grid
        num_rows (int): Number of rows in the grid
    """
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

    # Draw circles
    for i in range(num_rows):
        for j in range(num_columns):
            if i * num_columns + j < num_circles:
                # Calculate center positions
                x = left_margin + (j * (circle_diameter + x_spacing)) + circle_radius
                y = height - (top_margin + (i * (circle_diameter + y_spacing)) + circle_radius)
                c.circle(x, y, circle_radius, stroke=1, fill=0)

    # Save the PDF
    c.save()

if __name__ == "__main__":
    # Generate the PDF file with default parameters
    create_circle_pdf(OUTPUT_FILENAME)

    # Example of generating with custom parameters:
    # create_circle_pdf(
    #     "custom-circles.pdf",
    #     left_margin=0.75*inch,
    #     right_margin=0.75*inch,
    #     top_margin=1*inch,
    #     bottom_margin=1*inch,
    #     circle_diameter=0.8*inch,
    #     num_columns=6,
    #     num_rows=8
    # )