#include "chatlog.hpp"

char * loadFile(const char* path)
{
/*	std::ifstream file(path, std::ios::binary | std::ios::ate);
	int size = file.tellg();
	//cout << size << endl;
	char * data = new char[size];
	for(int i = 0; i < size; i++)
		data[i] = 0;
	file.seekg(0, std::ios::beg);
	file.read(data, size);
	cout << path << endl;
	cout << "\"" << (const char*) data << "\"" << endl;
	return data;
	*/

	FILE *fd;
	long len, r;
	char *str;

	if (!(fd = fopen(path, "r")))
	{
		fprintf(stderr, "Can't open file '%s' for reading\n", path);
		return NULL;
	}

	fseek(fd, 0, SEEK_END); // set position pointer to file end
	len = ftell(fd); // get the count of bytes that the pointer went
	fseek(fd, 0, SEEK_SET); // reset the pointer to the beginning of the file

	if (!(str = (char*)malloc(len * sizeof(char))))
	{
		fprintf(stderr, "Can't malloc space for '%s'\n", path);
		return NULL;
	}

	r = fread(str, sizeof(char), len, fd);

	str[r - 1] = '\0'; //make sure the string ends

	fclose(fd);
	//cout << path << endl;
	//cout << "\"" << (const char*) str << "\"" << endl;

	return str;
}

Chatlog::Chatlog(Window * window)
{
	font = new Image("data/font.png");
	kerning = new glm::vec2 [256];
	loadKerning();

	this->window = window;
	line_vertices = new TextVertices(window, font, kerning);

	message_list = new list<Message*>;
	vertices_list = new list<TextVertices*>;

	position = glm::vec2(-1, 1);

	programPointer = vertexPointer = fragmentPointer = 0;
	positionAttrib = texcoordAttrib = colorAttrib = 0;
	projectionUniform = viewUniform = modelUniform = fontTexUniform = 0;

	programPointer = glCreateProgram();

	char * vertSource = loadFile("data/chatshader.vert");
	vertexPointer = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexPointer, 1, (const char**)(&(vertSource)), NULL);
	glCompileShader(vertexPointer);
	delete [] vertSource;
	glAttachShader(programPointer, vertexPointer);

	char * fragSource = loadFile("data/chatshader.frag");
	fragmentPointer = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentPointer, 1, (const char**)(&(fragSource)), NULL);
	glCompileShader(fragmentPointer);
	delete [] fragSource;
	glAttachShader(programPointer, fragmentPointer);

	glLinkProgram(programPointer);

	glUseProgram(programPointer);
	glUseProgram(0);

	positionAttrib = glGetAttribLocation(programPointer, "position");
	texcoordAttrib = glGetAttribLocation(programPointer, "texcoord");
	colorAttrib = glGetAttribLocation(programPointer, "color");

	projectionUniform = glGetUniformLocation(programPointer, "projection");
	viewUniform = glGetUniformLocation(programPointer, "view");
	modelUniform = glGetUniformLocation(programPointer, "model");

	fontTexUniform = glGetUniformLocation(programPointer, "texture");

	cout << "ProjectionUniform: " << projectionUniform << " ViewUniform:" << viewUniform << " ModelUniform:" << modelUniform << " FontTexUniform:" << fontTexUniform << endl;
	cout << "ProgramPointer: " << programPointer << " VertexPointer: " << vertexPointer << " FragmentPointer: " << fragmentPointer << endl;
	cout << "PositionAttrib: " << positionAttrib << " TexCoordAttrib: " << texcoordAttrib << " ColorAttrib:" << colorAttrib << endl;

	view = glm::mat4(1.0);
	model = glm::mat4(1.0);
	projection = glm::ortho(-1.0, 1.0, 1.0, -1.0, -1.0, 1.0);
}

Chatlog::~Chatlog()
{
	glUseProgram(0);
	glDeleteShader(fragmentPointer);
	glDeleteShader(vertexPointer);
	glDeleteProgram(programPointer);

	delete font;
	delete [] kerning;

	for(list<Message*>::iterator it = message_list->begin(); it != message_list->end(); it++)
	{
		delete *it;
	}
	delete message_list;
}

void Chatlog::add (Message * msg)
{
	message_list->push_back(msg);
	TextVertices * tv  = new TextVertices(window, font, kerning);
	tv->upload(msg->by + string(": ") + msg->text, 1.5);
	vertices_list->push_back(tv);
}

void Chatlog::draw(bool draw_input_box)
{
	//cout << "draw" << endl;
	glUseProgram(programPointer);
	glUniform1i(fontTexUniform, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, font->getGlPointer());

	glm::vec2 position = this->position;

	if(draw_input_box && line_vertices->getVertexCount() > 0)
	{
		position.y -= line_vertices->getSize()->y;

		glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(projection));
		glm::mat4 translated_view = glm::translate(view, glm::vec3(position, 0.0));
		glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(translated_view));
		glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(model));

		glBindBuffer(GL_ARRAY_BUFFER, line_vertices->getPointer());
		glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(TextVertices::vertex_t), (void*)(0));
		glVertexAttribPointer(texcoordAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(TextVertices::vertex_t), (void*)(sizeof(glm::vec3)));
		glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(TextVertices::vertex_t), (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);


		glBindBuffer(GL_ARRAY_BUFFER, line_vertices->getPointer());
		glEnableVertexAttribArray(positionAttrib); glEnableVertexAttribArray(texcoordAttrib); glEnableVertexAttribArray(colorAttrib);

		glDrawArrays(GL_QUADS, 0, line_vertices->getVertexCount());

		glDisableVertexAttribArray(positionAttrib); glDisableVertexAttribArray(texcoordAttrib); glDisableVertexAttribArray(colorAttrib);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	for(list<TextVertices*>::iterator it = vertices_list->end(); it != vertices_list->begin();)
	{
		--it; //are you fucking kidding me? past-the-end iterator? seriously, i can't believe it. and you can't even use "list->end() - 1" because the dammn STL has no such thing for a list. why? because it would be O(n) instead of O(1) grrrrr
		position.y -= (*it)->getSize()->y;//TODO add check if we are beyond size, and ignore the remaining lines

		glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(projection));
		glm::mat4 translated_view = glm::translate(view, glm::vec3(position, 0.0));
		glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(translated_view));
		glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(model));

		glBindBuffer(GL_ARRAY_BUFFER, (*it)->getPointer());
		glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(TextVertices::vertex_t), (void*)(0));
		glVertexAttribPointer(texcoordAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(TextVertices::vertex_t), (void*)(sizeof(glm::vec3)));
		glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(TextVertices::vertex_t), (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);


		glBindBuffer(GL_ARRAY_BUFFER, (*it)->getPointer());
		glEnableVertexAttribArray(positionAttrib); glEnableVertexAttribArray(texcoordAttrib); glEnableVertexAttribArray(colorAttrib);

		glDrawArrays(GL_QUADS, 0, (*it)->getVertexCount());

		glDisableVertexAttribArray(positionAttrib); glDisableVertexAttribArray(texcoordAttrib); glDisableVertexAttribArray(colorAttrib);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

void Chatlog::setLine(const string * input)
{
	//this->line = input;
	string line = string("> ") + *input;
	line_vertices->upload(line, 1.5);
}

//private
void Chatlog::loadKerning()
{
	for(unsigned int row = 0; row <= 15; row++)
	{
		for(unsigned int col = 0; col <= 15; col++)
		{
			kerning[(row * 16) + col] = glm::vec2(0, 0); // initialize array
			//cout << "char (" << (row*16)+col << ")\n";

			//left to right
			for(unsigned int pixelX = 0; pixelX < 8; pixelX++)
			{
				for(unsigned int pixelY = 0; pixelY < 8; pixelY++)
				{
					if(font->getPixel((col * 8) + pixelX, (row * 8) + pixelY) != 0)
					{
						//cout << "from left in cell @ [" << row << ", " << col << "] pixel at [" << pixelX << ", " << pixelY << "] is not empty! (absolute: [" << (col*8)+pixelX << ", " << (row*8)+pixelY << "]" << endl;
						kerning[(row * 16) + col].x = pixelX;
						//break loops
						pixelX = 8;
						pixelY = 8;
					}
				}
			}

			//right to left
			for(int pixelX = 7; pixelX >= 0; pixelX--)
			{
				for(int pixelY = 7; pixelY >= 0; pixelY--)
				{
					if(font->getPixel((col * 8) + pixelX, (row * 8) + pixelY) != 0)
					{
						//cout << "from right in cell @ [" << row << ", " << col << "] pixel at [" << pixelX << ", " << pixelY << "] is not empty! (absolute: [" << (col*8)+pixelX << ", " << (row*8)+pixelY << "]" << endl;
						kerning[(row * 16) + col].y = 7 - pixelX;
						//break loops
						pixelX = -1;
						pixelY = -1;
					}
				}
			}
			//cout << endl;
		}
	}
}
